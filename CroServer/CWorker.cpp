#include "CWorker.h"

#include <iostream>

void CWorker::run(std::unique_ptr<tcp::socket> socket){
	while (true) {
		asio::error_code ec;
		std::array<char, 6> arr;
		asio::read(*socket, asio::buffer(arr), ec);
		asio::write(*socket, asio::buffer(arr), ec);
		
		if (ec) {
			std::cout << "DISCONNECTIO" << std::endl;
			break;
		}
	}
}