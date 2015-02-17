#pragma once
#include <QObject>
#include <QtXml>
#include <comutil.h>
#import <msxml6.dll>
class CAPIManager : public QObject{
	Q_OBJECT
public:
	CAPIManager();
	~CAPIManager();
public slots:
	void postReceiver(QString);
	void authSlot(QString, QString);
signals:
	void gotData(QString);
	void authResult(bool);
	void forwardAnswerToParse(QString);
	void forwardErrorToParse(QString);
private:
	void CAPIManager::queryRequest(QString id);
	const QString m_serverIp{ "127.0.0.1" };
};