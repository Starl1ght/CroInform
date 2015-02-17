#pragma once
#include "ICustomWidget.h"
class CDoubleCheckbox : public ICustomWidget {
	Q_OBJECT
public:
	CDoubleCheckbox(QWidget* widget, QDomNode* node);
	~CDoubleCheckbox() = default;
	QString getPostData() override;
	void reposition(int x, int y) override;
	int getYforNextWidget() override;

public slots:
	void checkbox1Click(int);
	void checkbox2Click(int);

private:
	std::unique_ptr<QLabel> m_label1;
	std::unique_ptr<QLabel> m_label2;
	std::unique_ptr<QCheckBox> m_checkbox1;
	std::unique_ptr<QCheckBox> m_checkbox2;
	QString m_postData1;
	QString m_postData2;
};