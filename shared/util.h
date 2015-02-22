#pragma once
#include <QtCore>
#include <QtXml>
#include <iostream>
#include <asio.h>

using socket_ptr = std::unique_ptr<tcp::socket>;
using packet_header = std::array<char, 4>;

namespace util{
	inline QByteArray toRu(const QString & str){
		QTextCodec *codec = QTextCodec::codecForName("cp1251");
		return codec->fromUnicode(str);
	}
	inline QString toQstr(const QByteArray& arr){
		QTextCodec *codec = QTextCodec::codecForName("cp1251");
		return codec->toUnicode(arr);
	}
	inline QString toQstr(char* str){
		QByteArray arr(str);
		return util::toQstr(arr);
	}
	inline uint32_t asio_calc_size(const packet_header & arr){
		uint32_t rez{ 0 };
		rez += arr.at(0) * 256 * 256 * 256;
		rez += arr.at(1) * 256 * 256;
		rez += arr.at(2) * 256;
		rez += arr.at(3);
		return rez;
	}
	inline packet_header asio_packet_size(uint32_t size){
		packet_header rez;
		rez[0] = static_cast<char>(size & 0xFF000000);
		rez[1] = static_cast<char>(size & 0x00FF0000);
		rez[2] = static_cast<char>(size & 0x0000FF00);
		rez[3] = static_cast<char>(size & 0x000000FF);
		return rez;
	}
	inline asio::error_code asio_read(const socket_ptr & socket, QByteArray & toRead){
		asio::error_code err;
		packet_header header;
		asio::read(*socket, asio::buffer(header), err);

		if (err) {
			return err;
		}

		uint32_t packSize = util::asio_calc_size(header);
		// I cannot do this without new char*. Spent ~2 hours, futile... :(
		char* buf = new char[packSize];
		asio::read(*socket, asio::buffer(buf, packSize), err);
		toRead.clear();
		toRead.append(buf, packSize);
		delete[] buf;
		return err;
	}
	inline asio::error_code asio_write(const socket_ptr & socket, const QByteArray & toWrite){
		asio::error_code err;

		packet_header pkt = util::asio_packet_size(toWrite.length());
		asio::write(*socket, asio::buffer(pkt), err);

		if (err) {
			return err;
		}

		asio::write(*socket, asio::buffer(toWrite.data(), toWrite.length()), err);
		return err;
	}
}

enum class authResult {
	admin,
	user,
	fail
};