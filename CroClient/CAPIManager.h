#pragma once
#include <util.h>
#include <QObject>
#include <QtXml>
#include <QMessageBox>

class CAPIManager : public QObject{
	Q_OBJECT
public:
	CAPIManager();
	~CAPIManager() = default;
public slots:
	void postReceiver(QString);
	void authSlot(QString, QString);
signals:
	void gotData(QString);
	void authResultSignal(authResult);
	void forwardAnswerToParse(QString);
	void forwardErrorToParse(QString);
private:
	void CAPIManager::queryRequest(QString id);

	asio::io_service io;
	socket_ptr m_socket;
};