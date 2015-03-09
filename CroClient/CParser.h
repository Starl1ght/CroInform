#pragma once
#include <QObject>
#include <QtXML>
#include <QApplication>
#include <QMessageBox>
#include <QMap>
#include "util.h"
class CParser : public QObject{
	Q_OBJECT
public:
	CParser();
	~CParser() = default;
public slots:
	void parseAnswer(QString);
	void parseError(QString);
signals:
	void parsed(QString);
	void unblockUI();
	void queryAgain(QString);
private:
	const QString & translate(const QString & toTr);
	QMap<QString, QString> m_map;
};