#pragma once
#include <QObject>
#include <QtXML>
#include "util.h"
class CParser : public QObject{
	Q_OBJECT
public:
	CParser() = default;
	~CParser() = default;
public slots:
	void parseAnswer(QString);
	void parseError(QString);
signals:
	void parsed(QString);
	void unblockUI();
};