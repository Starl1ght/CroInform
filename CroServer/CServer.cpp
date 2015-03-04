#include "CServer.h"
#include <iostream>

void main(int argc, char *argv[]){

	QCoreApplication app(argc, argv);
	CServer server;
}

CServer::CServer(){
	m_api = new CPlatformApiLayer();
	if (!m_sql.connect()) {
		exit(-2);
	}
	std::cout << "DB connect OK" << std::endl;
	
	QString log, pass;
	m_sql.fetchApiCredentials(&log, &pass);

	if (!m_api->auth(log, pass)) {
		std::cout << "API cannot auth\connect" << std::endl;
		exit(-3);
	}

	std::cout << "CroAPI connect OK" << std::endl;

	asio::io_service io_service;
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3228));
	std::cout << "Accepting connections..." << std::endl;

	std::vector<CWorker> vec;
	vec.push_back(CWorker());
	QObject::connect(&vec.back(), &CWorker::checkUserLogin, &m_sql, &CSqlLayer::validateUserCredentials, Qt::DirectConnection);
	QObject::connect(&m_sql, &CSqlLayer::authSignal, &vec.back(), &CWorker::infoUserLogin, Qt::DirectConnection);
	while (true) {
		tcp::socket* socket = new tcp::socket(io_service);
		acceptor.accept(*socket);
		std::cout << "Connected IP: " << socket->remote_endpoint().address().to_string() << std::endl;

		
		std::thread thrs([&] { 
			vec.back().run(socket, m_api);
			std::cout << "Disconnected: " << socket->remote_endpoint().address().to_string() << std::endl;
		});
		thrs.detach();
	}
}

CServer::~CServer(){
	delete m_api;
}
