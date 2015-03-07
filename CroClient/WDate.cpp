#include "WDate.h"

CDate::CDate(QWidget* widget, QDomNode* node){
	m_date.reset(new QDateEdit(widget));
	m_label.reset(new QLabel(widget));

	m_label->setText(node->toElement().attribute("caption"));
	m_postData = node->toElement().attribute("post");

	m_date->show();
	m_label->show();

}

QString CDate::getPostData(){
	QString ret{m_postData};
	if (m_date->date().day() < 10) {
		ret += "0";
	}
	ret += QString::number(m_date->date().day());
	ret += ".";

	if (m_date->date().month() < 10) {
		ret += "0";
	}
	ret += QString::number(m_date->date().month());
	ret += ".";
	ret += QString::number(m_date->date().year());

	return ret;
}

void CDate::reposition(int x, int y){
	m_label->setGeometry(x, y, 180 - 20, 15);
	m_date->setGeometry(x, y + 20, 180 - 20, 15);
}

int CDate::getYforNextWidget(){
	return m_date->geometry().y() + 20; 
}