#include "WDoubleCheckbox.h"

CDoubleCheckbox::CDoubleCheckbox(QWidget* widget, QDomNode* node){
	m_checkbox1.reset(new QCheckBox(widget));
	m_checkbox2.reset(new QCheckBox(widget));

	m_label1.reset(new QLabel(widget));
	m_label2.reset(new QLabel(widget));

	m_label1->setText(node->firstChildElement().attribute("caption"));
	m_postData1 = node->firstChildElement().attribute("post");

	m_label2->setText(node->lastChildElement().attribute("caption"));
	m_postData2 = node->lastChildElement().attribute("post");

	QObject::connect(m_checkbox1.get(), &QCheckBox::stateChanged, this, &CDoubleCheckbox::checkbox1Click);
	QObject::connect(m_checkbox2.get(), &QCheckBox::stateChanged, this, &CDoubleCheckbox::checkbox2Click);

	m_checkbox1->show();
	m_checkbox2->show();
	m_label1->show();
	m_label2->show();
}
QString CDoubleCheckbox::getPostData(){
	if (m_checkbox1->checkState() == Qt::Checked) {
		return m_postData1;
	}
	if (m_checkbox2->checkState() == Qt::Checked) {
		return m_postData2;
	}
	return "";
}
void CDoubleCheckbox::reposition(int x, int y){
	m_checkbox1->setGeometry(x, y, 15, 15);
	m_label1->setGeometry(x + 20, y, 180 - 20, 15);

	m_checkbox2->setGeometry(x, y + 20, 15, 15);
	m_label2->setGeometry(x + 20, y + 20, 180 - 20, 15);

}
int CDoubleCheckbox::getYforNextWidget(){
	return m_checkbox2->geometry().y() + 20;
}

void CDoubleCheckbox::checkbox1Click(int state){
	if (state == Qt::Checked) {
		m_checkbox2->setCheckState(Qt::Unchecked);
	}
}
void CDoubleCheckbox::checkbox2Click(int state){
	if (state == Qt::Checked) {
		m_checkbox1->setCheckState(Qt::Unchecked);
	}
}