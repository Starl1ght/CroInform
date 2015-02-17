#include "WLineCheckbox.h"

CLineCheckbox::CLineCheckbox(QWidget* widget, QDomNode* node){
	m_lineEdit.reset(new QLineEdit(widget));
	m_checkBox.reset(new QCheckBox(widget));
	m_label.reset(new QLabel(widget));
	
	m_checkedCaption = node->namedItem("checked").toElement().attribute("caption");
	m_checkedPostData = node->namedItem("checked").toElement().attribute("post");
	m_uncheckedCaption = node->namedItem("unchecked").toElement().attribute("caption");
	m_uncheckedPostData = node->namedItem("unchecked").toElement().attribute("post");

	QObject::connect(m_checkBox.get(), &QCheckBox::stateChanged, this, &CLineCheckbox::checkboxChecked);
	m_checkBox->setChecked(false);

	m_label->setText(m_uncheckedCaption);
	m_lineEdit->show();
	m_checkBox->show();
	m_label->show();
}

void CLineCheckbox::reposition(int x, int y){
	m_checkBox->setGeometry(x, y, 15, 15);
	m_label->setGeometry(x + 20, y, 160, 15);
	m_lineEdit->setGeometry(x, y + 20, 160, 15);
}

int CLineCheckbox::getYforNextWidget(){
	return m_lineEdit->geometry().y() + 20;
}


QString CLineCheckbox::getPostData(){
	if (m_checkBox->checkState() == Qt::Unchecked) {
		return m_uncheckedPostData + m_lineEdit->text();
	} else {
		return m_checkedPostData + m_lineEdit->text();
	}
}

void CLineCheckbox::checkboxChecked(int state){
	if (state == Qt::Unchecked) {
		m_label->setText(m_uncheckedCaption);
	} else {
		m_label->setText(m_checkedCaption);
	}
}