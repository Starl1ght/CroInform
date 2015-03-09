#pragma once
#include <util.h>
#include <memory>
#include "CPlatformApiLayer.h"

class CWorker : public QObject{
	Q_OBJECT
public:
	CWorker(){};
	~CWorker(){};
	CWorker(const CWorker &){};
	void run(tcp::socket* sock, CPlatformApiLayer* api);

public slots:
	void infoUserLogin(authResult);
	
signals:
	void checkUserLogin(QString, QString);
	
private:
	void readSocket(QByteArray & toRead);
	void writeSocket(const QByteArray & toWrite);
	void mainLoop();

	QString m_login{"Not authenticated"};

	authResult m_permissions{ authResult::fail };
	socket_ptr m_socket{ nullptr };
	asio::error_code m_error;
	bool m_socketOk{ true };

	CPlatformApiLayer *m_api;
};