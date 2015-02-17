#include "util.h"
QByteArray util::toRu(const QString & str){
	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	return codec->fromUnicode(str);
}
QString util::toQstr(const QByteArray & arr){
	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	return codec->toUnicode(arr);
}
QString util::toQstr(char* str){
	QByteArray arr(str);
	return util::toQstr(arr);
}

uint32_t util::asio_calc_size(const packet_size & arr){
	uint32_t rez;
	rez += arr.at(0) * pow(2, 24);
	rez += arr.at(1) * pow(2, 16);
	rez += arr.at(2) * pow(2, 8);
	rez += arr.at(3);
	return rez;
}
packet_size util::asio_packet_size(uint32_t size){
	packet_size rez;
	rez[0] = size & 0xFF000000;
	rez[1] = size & 0x00FF0000;
	rez[2] = size & 0x0000FF00;
	rez[3] = size & 0x000000FF;
	return rez;
}


asio::error_code util::asio_read(const socket_ptr & socket, QByteArray* toRead){
	asio::error_code err;
	packet_size size;
	asio::read(*socket, size, err);

	if (err) {
		return err;
	}
	
	asio::read(*socket, asio::buffer(toRead->data(), util::asio_calc_size(size)), err);
	return err;
}
asio::error_code util::asio_write(const socket_ptr & socket, const QByteArray & toWrite){

}