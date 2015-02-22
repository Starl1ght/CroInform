#include "CAPIManager.h"

CAPIManager::CAPIManager(){
	Sleep(1000);
	tcp::endpoint ep{ asio::ip::address_v4::from_string("127.0.0.1"), 3228 };
	m_socket->connect(ep);
	
}


void CAPIManager::postReceiver(QString){

}

void CAPIManager::authSlot(QString login, QString password){
	QByteArray arr;
	
	QDomDocument doc;
	QDomElement root = doc.createElement("auth");
	root.setAttribute("login", login);
	root.setAttribute("password", password);
	doc.appendChild(root);

	arr = util::toRu(doc.toString());
		
	asio::error_code ec;
	ec = util::asio_write(m_socket, arr);
	if (ec) {
		return; // lol
	}

	util::asio_read(m_socket, arr);
	doc.setContent(arr);
	if (doc.documentElement().tagName() == "auth") {
		if (doc.documentElement().attribute("result") == "fail") {
			emit authResultSignal(authResult::fail);
		} 
		if (doc.documentElement().attribute("result") == "admin") {
			emit authResultSignal(authResult::admin);
		}
		if (doc.documentElement().attribute("result") == "user") {
			emit authResultSignal(authResult::user);
		}
	}
}

void CAPIManager::queryRequest(QString id) {

}