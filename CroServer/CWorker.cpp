#include "CWorker.h"
void CWorker::run(tcp::socket* sock){
	m_socket = socket_ptr(sock);
	while (m_socketOk) {
		QByteArray reply;
		readSocket(reply);
		QDomDocument doc;
		doc.setContent(util::toQstr(reply));
		if (doc.documentElement().tagName() == "auth") {
			emit checkUserLogin(doc.documentElement().attribute("login"), doc.documentElement().attribute("password"));
		}
	}
}

void CWorker::infoUserLogin(authResult rez){
	QDomDocument doc;
	QDomElement elem = doc.createElement("auth");
	
	switch (rez) {
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
	}


}
void CWorker::writeSocket(const QByteArray & toWrite){
	auto ec = util::asio_write(m_socket, toWrite);
	if (ec) {
		m_socketOk = false;
	}
}