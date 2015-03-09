#pragma once
#include "ICustomWidget.h"
class CHidden : public ICustomWidget {
	Q_OBJECT
public:
	CHidden(QWidget* widget, QDomNode* node);
	~CHidden();
	QString getPostData() override;
	void reposition(int x, int y) override;
	int getYforNextWidget() override;

private:
	QString m_postData;
	int m_stubY;
};