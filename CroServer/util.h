#pragma once
#include <QtCore>
#include <QtXml>
#include <iostream>
#include "asio.h""

using socket_ptr = std::unique_ptr<tcp::socket>;
using packet_size = std::array<unsigned char, 4>;


namespace util{
	QByteArray toRu(const QString & str);
	QString toQstr(const QByteArray& arr);
	QString toQstr(char* str);

	asio::error_code asio_read(const socket_ptr & socket, QByteArray* toRead);
	asio::error_code asio_write(const socket_ptr & socket, const QByteArray & toWrite);
	
	uint32_t asio_calc_size(const packet_size & arr);
	packet_size asio_packet_size(uint32_t size);
}


enum class authResult {
	admin,
	user,
	fail
};