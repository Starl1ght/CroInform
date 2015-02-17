#pragma once
#include "ICustomWidget.h"
class CLineCheckbox : public ICustomWidget {
	Q_OBJECT
public:
	CLineCheckbox(QWidget* widget, QDomNode* node);
	~CLineCheckbox() = default;
	QString getPostData() override;
	void reposition(int x, int y) override;
	int getYforNextWidget() override;

public slots:
	void checkboxChecked(int);

private:
	std::unique_ptr<QLineEdit> m_lineEdit;
	std::unique_ptr<QCheckBox> m_checkBox;
	std::unique_ptr<QLabel> m_label;

	QString m_checkedCaption;
	QString m_uncheckedCaption;
	QString m_checkedPostData;
	QString m_uncheckedPostData;
};

