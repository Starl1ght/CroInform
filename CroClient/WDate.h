#pragma once
#include "ICustomWidget.h"

class CDate : public ICustomWidget {
	Q_OBJECT
public:
	CDate(QWidget* widget, QDomNode* node);
	~CDate();
	QString getPostData() override;
	void reposition(int x, int y) override;
	int getYforNextWidget() override;
};