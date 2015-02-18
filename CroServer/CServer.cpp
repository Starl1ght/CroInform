#include "CServer.h"
#include <iostream>

void main(int argc, char *argv[]){

	QCoreApplication app(argc, argv);
	CServer server;
}

CServer::CServer(){
	if (!m_sql.connect()) {
		exit(-2);
	}
	std::cout << "DB connect OK" << std::endl;
	
	QString log, pass;
	m_sql.fetchApiCredentials(&log, &pass);
	if (!m_api.auth(log, pass)) {
		exit(-3);
	}
	std::cout << "CroAPI connect OK" << std::endl;


	std::vector<std::thread*> beba;
	asio::io_service io_service;
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3228));
	std::cout << "Accepting connections..." << std::endl;

	std::vector<CWorker> vec;
	
	while (true) {
		tcp::socket* socket = new tcp::socket(io_service);
		acceptor.accept(*socket);
		std::cout << "Connected IP: " << socket->remote_endpoint().address().to_string() << std::endl;

		vec.push_back(CWorker());
		std::thread thr([&] { vec.back().run(std::unique_ptr<tcp::socket>(socket)); });
		thr.detach();
	}
}


//CServer::run();