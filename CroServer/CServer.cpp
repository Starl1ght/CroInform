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
		std::cout << "API cannot auth\\connect" << std::endl;
		exit(-3);
	}

	std::cout << "CroAPI connect OK" << std::endl;

	asio::io_service io_service;
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3228));
	std::cout << "Accepting connections..." << std::endl;

	std::vector<CWorker> workerVec;
//	std::vector<std::thread> threadVec;
	

	while (true) {
		tcp::socket* socket = new tcp::socket(io_service);
		acceptor.accept(*socket);
		std::cout << "Connected IP: " << socket->remote_endpoint().address().to_string() << std::endl;

		workerVec.push_back(CWorker());
		QObject::connect(&workerVec.back(), &CWorker::checkUserLogin, &m_sql, &CSqlLayer::validateUserCredentials, Qt::DirectConnection);
		QObject::connect(&m_sql, &CSqlLayer::authSignal, &workerVec.back(), &CWorker::infoUserLogin, Qt::DirectConnection);
		std::thread thr(&CWorker::run, &workerVec.back(), socket, m_api);
		thr.detach();
	}
}

CServer::~CServer(){
	delete m_api;
}
