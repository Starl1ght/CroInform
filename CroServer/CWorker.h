#pragma once
#include "asio.h"
#include <memory>
class CWorker{
public:
	CWorker() = default;

	void run(std::unique_ptr<tcp::socket> socket);
};