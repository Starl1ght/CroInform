#include "CAPIManager.h"

CAPIManager::CAPIManager(){
	tcp::endpoint ep{ asio::ip::address_v4::from_string("127.0.0.1"), 3228 };
	m_socket->connect(ep);
	
}


void CAPIManager::postReceiver(QString){

}
void CAPIManager::authSlot(QString login, QString password){

	QByteArray toSend;
	
	toSend += util::toRu(login);
	toSend += util::toRu(password);
	
	asio::error_code ec;
	if (ec) {
		return;
	}

	util::asio_write(m_socket, toSend);
}

void CAPIManager::queryRequest(QString id) {

}