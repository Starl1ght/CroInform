#pragma once
#include <util.h>
#include <memory>
class CWorker{
public:
	CWorker() = default;

	void run(std::unique_ptr<tcp::socket> socket);
};