#include "CParser.h"

void CParser::parseAnswer(QString answer){

	QDomDocument doc;
	doc.setContent(answer);

	



	emit parsed(doc.toString());
	emit unblockUI();
}

void CParser::parseError(QString error){
	QDomDocument doc;
	doc.setContent(error);

	error = util::toQstr("<html><body><h1>��������� -> ");
	error += doc.documentElement().firstChildElement().tagName();
	error += util::toQstr("</h1><br> ���. ���������� -> ");
	error += doc.documentElement().firstChildElement().text();
	error += "</body></html>";


	


	emit parsed(error);
	emit unblockUI();
}