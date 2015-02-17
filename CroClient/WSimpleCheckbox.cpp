#include "WSimpleCheckbox.h"

CSimpleCheckbox::CSimpleCheckbox(QWidget* widget, QDomNode* node){
	m_checkbox.reset(new QCheckBox(widget));
	m_label.reset(new QLabel(widget));
	
	m_checkedPostData = node->toElement().attribute("post");
	m_label->setText(node->toElement().attribute("caption"));

	m_label->show();
	m_checkbox->show();
}
QString CSimpleCheckbox::getPostData(){
	if (m_checkbox->checkState() == Qt::Checked) {
		return m_checkedPostData;
	} else {
		return "";
	}
}
void CSimpleCheckbox::reposition(int x, int y){
	m_checkbox->setGeometry(x,y,15,15);
	m_label->setGeometry(x + 20, y, 180 - 20, 15);
}
int CSimpleCheckbox::getYforNextWidget(){
	return m_checkbox->geometry().y() + 20;
}