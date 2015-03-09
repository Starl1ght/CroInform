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

	// TODO - remove
	QFile outFile("c:\\bob4.xml");
	outFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream stream(&outFile);
	stream << doc.toString();
	// !REMOVE

	auto root = doc.documentElement();
	QString html{ "<html>" };

	html = html + "<h1>" + translate("StatusRequest") + " -> ";
	if (root.firstChildElement("StatusRequest").text() == "1") {
		html += translate("_found");
	}
	if (root.firstChildElement("StatusRequest").text() == "3") {
		html += translate("_searching");
	}
	if (root.firstChildElement("StatusRequest").text() == "0" || root.firstChildElement("StatusRequest").text() == "") {
		html += translate("_notfound");
	}
	html += "</h1><br>";



	html = html + "<h1>"+ translate("StatusDtl") + "</h1><br>";
	for (auto elem = root.firstChildElement("StatusDtl").firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement()) {
		html = html + translate(elem.tagName()) + " -> ";
		if (elem.text() == "1") {
			html += translate("_found");
		}
		if (elem.text() == "3") {
			html += translate("_searching");
		}
		if (elem.text() == "0" || elem.text() == "") {
			html += translate("_notfound");
		}
		html += "<br>";
	}

	html += "<h1>" + translate("ResponseData") + "</h1><br>";
	for (auto elem = root.firstChildElement("ResponseData").firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement()) {
		html = html + CParser::translate(elem.tagName()) + " -> " + elem.text() + "<br>";
	}
	
	if (root.firstChildElement("Documents").tagName() == "Documents") {
		if (root.firstChildElement("Documents").firstChildElement("AnswerHtml").tagName() == "AnswerHtml") {
			html += "<h1>" + translate("AnswerHtml") + "</h1><br>";
			html += root.firstChildElement("Documents").firstChildElement("AnswerHtml").toCDATASection().nodeValue();
			html += root.firstChildElement("Documents").firstChildElement("AnswerHtml").text();

			
				//.firstChildEment("AnswerHTML").toCDATASection().nodeValue();
		}
	}

	emit parsed(html);

	if (root.firstChildElement("StatusRequest").text() == "3") { // searching
		for (int i = 0; i < 40; ++i) {
			Sleep(100);
			QApplication::processEvents();
		}
		emit queryAgain(root.firstChildElement("RequestNumber").text());
	} else {
		emit unblockUI();
	}
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