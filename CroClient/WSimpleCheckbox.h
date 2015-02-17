#pragma once
#include "ICustomWidget.h"
class CSimpleCheckbox : public ICustomWidget {
	Q_OBJECT
public:
	CSimpleCheckbox(QWidget* widget, QDomNode* node);
	~CSimpleCheckbox() = default;
	QString getPostData() override;
	void reposition(int x, int y) override;
	int getYforNextWidget() override;
	
private:
	std::unique_ptr<QLabel> m_label;
	std::unique_ptr<QCheckBox> m_checkbox;

	QString m_caption;
	QString m_checkedPostData;
};