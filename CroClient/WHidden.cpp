#include "WHidden.h"

CHidden::CHidden(QWidget* widget, QDomNode* node){
	widget; // mitigate warning
	m_postData = node->toElement().attribute("post");
}

CHidden::~CHidden(){};

QString CHidden::getPostData(){
	return m_postData;
}

void CHidden::reposition(int x, int y){
	x; // mitigate warning
	m_stubY = y;
}
int CHidden::getYforNextWidget(){
	return m_stubY;
}

