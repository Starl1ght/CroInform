#include "CParser.h"

CParser::CParser(){
	QDomDocument doc;
	QFile file("msg.xml");
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(nullptr, QString::fromLocal8Bit("Ошибка"), util::toQstr("Файл msg.xml не найден"));
		exit(-3421);
	}

	if (!doc.setContent(&file)) {
		QMessageBox::information(nullptr, QString::fromLocal8Bit("Ошибка"), util::toQstr("Ошибка в msg.xml"));
		exit(-4321);
	}

	QDomNodeList nodes = doc.documentElement().elementsByTagName("tr");
	for (int i = 0; i < nodes.count(); i++) {
		m_map.insert(
			nodes.at(i).toElement().attribute("from"),
			nodes.at(i).toElement().attribute("to")
		);
	}

	file.close();
}


void CParser::parseAnswer(QString answer){

	QDomDocument doc;
	doc.setContent(answer);

	
	emit parsed(doc.toString());
	emit unblockUI();
}

void CParser::parseError(QString error){
	QDomDocument doc;
	doc.setContent(error);

	error = util::toQstr("<html><body><h1>Результат -> ");
	error += CParser::translate(doc.documentElement().firstChildElement().tagName());
	error += util::toQstr("</h1><br> Доп. информация -> ");
	error += CParser::translate(doc.documentElement().firstChildElement().text());
	error += "</body></html>";

	emit parsed(error);
	emit unblockUI();
}

const QString & CParser::translate(const QString & toTr){
	if (m_map[toTr] != "") {
		return m_map[toTr];
	}
	return toTr;
}