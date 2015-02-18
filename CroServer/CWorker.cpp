#include "CWorker.h"

#include <iostream>

void CWorker::run(std::unique_ptr<tcp::socket> socket){
	while (true) {
		QByteArray reply;
		auto ec = util::asio_read(socket, &reply);
							
		if (ec) {
			std::cout << "DISCONNECTIO" << std::endl;
			break;
		}
		std::cout << reply.toStdString() << std::endl;
	}
}