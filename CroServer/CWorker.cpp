#include "CWorker.h"


void CWorker::run(tcp::socket* sock, CPlatformApiLayer* api){
	asio::error_code ec;
	m_socket = socket_ptr(sock);
	QByteArray reply;
	QDomDocument doc;

	if (m_socketOk) { // if first packet is not auth - GTFO
		readSocket(reply);
		doc.setContent(reply);
		if (doc.documentElement().tagName() == "auth") {
			emit checkUserLogin(doc.documentElement().attribute("login"), doc.documentElement().attribute("password"));
		} else {
			m_socketOk = false;
			m_socket->close(ec);
		}
	}

	QString answer;
	while (m_socketOk && m_permissions != authResult::fail) {
		readSocket(reply);
		doc.setContent(util::toQstr(reply));
		
		if (doc.documentElement().tagName() == "post") { 
			api->requestApi(doc.documentElement().attribute("data"), answer);
			writeSocket(util::toRu(answer));
		}

		if (doc.documentElement().tagName() == "query") {
			api->queryApi(doc.documentElement().attribute("id"), answer);
			writeSocket(util::toRu(answer));
		}

		if (m_permissions == authResult::admin) {
			if (doc.documentElement().tagName() == "user") { 
				doc.documentElement().attribute("action");
			}
		}
	}
	m_socketOk = false;
	m_socket->close(ec);
}

void CWorker::infoUserLogin(authResult rez){
	QDomDocument doc;
	QDomElement elem = doc.createElement("auth");
	

	m_permissions = rez;

	switch (m_permissions) {
	case authResult::admin:
		elem.setAttribute("result", "admin");
		break;
	case authResult::user:
		elem.setAttribute("result", "user");
		break;
	case authResult::fail:
		elem.setAttribute("result", "fail");
		break;
	}
	doc.appendChild(elem);
	writeSocket(util::toRu(doc.toString()));
}

void CWorker::readSocket(QByteArray & toRead){
	auto ec = util::asio_read(m_socket, toRead);
	if (ec) {
		m_socketOk = false;
		m_socket->close(ec);
	}


}
void CWorker::writeSocket(const QByteArray & toWrite){
	auto ec = util::asio_write(m_socket, toWrite);
	if (ec) {
		m_socketOk = false;
		m_socket->close(ec);
	}
}