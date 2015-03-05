#include "CWorker.h"


void CWorker::run(tcp::socket* sock, CPlatformApiLayer* api){
	asio::error_code ec;
	m_api = api;
	m_socket = socket_ptr(sock);
	QByteArray reply;
	QDomDocument doc;

	if (m_socketOk) { // if first packet is not auth - GTFO
		readSocket(reply);
		doc.setContent(reply);
		if (doc.documentElement().tagName() == "auth") {
			m_login = doc.documentElement().attribute("login");
			emit checkUserLogin(m_login, doc.documentElement().attribute("password"));
		} else {
			m_socketOk = false;
			m_socket->close(ec);
		}
	}
}

void CWorker::mainLoop(){
	QByteArray reply;
	QDomDocument doc;
	QString answer;
	
	while (m_socketOk && m_permissions != authResult::fail) {
		readSocket(reply);
		doc.setContent(util::toQstr(reply));

		if (doc.documentElement().tagName() == "post") {
			m_api->requestApi(doc.documentElement().attribute("data"), answer);
			writeSocket(util::toRu(answer));
		}

		if (doc.documentElement().tagName() == "query") {
			m_api->queryApi(doc.documentElement().attribute("id"), answer);
			writeSocket(util::toRu(answer));
		}

		if (m_permissions == authResult::admin) {
			if (doc.documentElement().tagName() == "user") {
				doc.documentElement().attribute("action");
			}
		}
	}
	
	if (m_permissions == authResult::fail) {
		std::cout << "Disconnected: " << "Not Authenticated" << std::endl;
	} else {
		std::cout << "Disconnected: " << m_login.toStdString() << std::endl;
	}

	try{
		m_socket->close();
	} catch (std::exception & ec) {
		ec;
	}
}

void CWorker::infoUserLogin(authResult rez){
	QDomDocument doc;
	QDomElement elem = doc.createElement("auth");
	
	m_permissions = rez;

	switch (m_permissions) {
	case authResult::admin:
		elem.setAttribute("result", "admin");
		std::cout << "Authenticated: " << m_login.toStdString() << " as admin" << std::endl;
		break;
	case authResult::user:
		elem.setAttribute("result", "user");
		std::cout << "Authenticated: " << m_login.toStdString() << " as user"<< std::endl;
		break;
	case authResult::fail:
		elem.setAttribute("result", "fail");
		break;
	}
	doc.appendChild(elem);
	writeSocket(util::toRu(doc.toString()));

	mainLoop();
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