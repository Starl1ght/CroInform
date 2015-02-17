#include "WSimpleLine.h"

CSimpleLine::CSimpleLine(QWidget* widget, QDomNode* node){
	m_lineEdit.reset(new QLineEdit(widget));
	m_label.reset(new QLabel(widget));

	m_label->setText(node->toElement().attribute("caption"));
	m_postData = node->toElement().attribute("post");
	m_label->show();
	m_lineEdit->show();
}

QString CSimpleLine::getPostData(){
	if (m_lineEdit->text() == "") {
		return "";
	} else {
		return m_postData + m_lineEdit->text();
	}
}
void CSimpleLine::reposition(int x, int y){
	m_label->setGeometry(x, y, 160, 15);
	m_lineEdit->setGeometry(x, y + 20, 160, 15);
}

int CSimpleLine::getYforNextWidget(){
	return m_lineEdit->geometry().y() + 20;
}