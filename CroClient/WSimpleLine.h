#pragma once
#include "ICustomWidget.h"
class CSimpleLine : public ICustomWidget {
	Q_OBJECT
public:
	CSimpleLine(QWidget* widget, QDomNode* node);
	~CSimpleLine() = default;
	QString getPostData() override;
	void reposition(int x, int y) override;
	int getYforNextWidget() override;

public:
	std::unique_ptr<QLineEdit> m_lineEdit;
	std::unique_ptr<QLabel> m_label;
	QString m_postData;
};