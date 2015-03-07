#pragma once
#include "ICustomWidget.h"
#include <QLabel>
#include <QDateEdit>
class CDate : public ICustomWidget {
	Q_OBJECT
public:
	CDate(QWidget* widget, QDomNode* node);
	~CDate() = default;
	QString getPostData() override;
	void reposition(int x, int y) override;
	int getYforNextWidget() override;

private:
	std::unique_ptr<QLabel> m_label;
	std::unique_ptr<QDateEdit> m_date;
	QString m_postData;
};