#pragma once
#include <util.h>
#include <memory>
class CWorker : public QObject{
	Q_OBJECT

public:
	CWorker(){};
	~CWorker(){};
	CWorker(const CWorker &){};
	void run(tcp::socket* sock);

public slots:
	void infoUserLogin(authResult);
	
signals:
	void checkUserLogin(QString, QString);

private:
	void readSocket(QByteArray & toRead);
	void writeSocket(const QByteArray & toWrite);
		
	socket_ptr m_socket{ nullptr };
	asio::error_code m_error;
	bool m_socketOk{ true };
};