#include "CMainWindow.h"

void main(int argc, char** argv){
	QApplication app(argc, argv);
	CMainWindow window;
	CAuthDialog dlg;
	CAPIManager api;
	CParser parser;

	// auth routine
	QObject::connect(&dlg, &CAuthDialog::authSignal, &api, &CAPIManager::authSlot);
	QObject::connect(&api, &CAPIManager::authResultSignal, &dlg, &CAuthDialog::authResultSlot);
	// if auth is successful -> show window
	QObject::connect(&dlg, &CAuthDialog::authSuccessful, &window, &CMainWindow::showWindow);
	// send request to API
	QObject::connect(&window, &CMainWindow::submitRequest, &api, &CAPIManager::postReceiver);
	// Interaction with parser
	QObject::connect(&parser, &CParser::unblockUI, &window, &CMainWindow::unblockUI);
	QObject::connect(&parser, &CParser::parsed, &window, &CMainWindow::getFormattedData);
	// Parser <-> API
	QObject::connect(&api, &CAPIManager::forwardAnswerToParse, &parser, &CParser::parseAnswer);
	QObject::connect(&api, &CAPIManager::forwardErrorToParse, &parser, &CParser::parseError);

	app.exec();
}

CMainWindow::CMainWindow(){
	initializeXML();
	initializeUI();
	QObject::connect(m_choiceComboBox.get(), SIGNAL(currentIndexChanged(int)), this, SLOT(comboboxChanged(int)));
	QObject::connect(m_submitButton.get(), &QPushButton::clicked, this, &CMainWindow::prepareRequest);
	this->resize(m_defWidth, m_defHeight);
	m_submitButton->setText(QString::fromLocal8Bit("Запрос"));
}
void CMainWindow::unblockUI(){
	m_choiceComboBox->setEnabled(true);
	m_submitButton->setEnabled(true);
}
void CMainWindow::getFormattedData(QString text){
	m_outputTextEdit->clear();
	m_outputTextEdit->insertHtml(text);
	//m_outputTextEdit->setText(text);
}
void CMainWindow::showWindow(){
	this->show();
}

void CMainWindow::resizeEvent(QResizeEvent * event){ 
	m_outputTextEdit->setGeometry(0, -1, event->size().width() - 200, event->size().height());
	m_choiceComboBox->setGeometry(event->size().width() - 180, 20, 160, 20);
	
	if (!m_widgetVec.empty()) {
		m_widgetVec.front()->reposition(event->size().width() - 180, 50);

		for (quint8 i = 1; i < m_widgetVec.size(); ++i) {
			m_widgetVec.at(i)->reposition(event->size().width() - 180, m_widgetVec.at(i - 1)->getYforNextWidget());
		}
	}
	m_submitButton->setGeometry(event->size().width() - 180, m_widgetVec.back()->getYforNextWidget(), 160, 30);
}

void CMainWindow::comboboxChanged(int index){
	rebuildUI(index);
}

void CMainWindow::closeEvent(QCloseEvent *event){
	event; // warning
	QApplication::quit();
}

void CMainWindow::prepareRequest(bool unused){
	unused; // warning
	m_choiceComboBox->setEnabled(false);
	m_submitButton->setEnabled(false);
	QString post{ "Type=Request&Event=" };
	post += QString::number(m_choiceComboBox->currentIndex() + 1);
	for (auto & ptr : m_widgetVec) {
		post += ptr->getPostData();
	}
	emit submitRequest(post);
}

void CMainWindow::initializeXML(){
	QFile file("ui.xml");
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), util::toQstr("Файл ui.xml не найден"));
		exit(-1);
	}

	if (!m_doc.setContent(&file)) {
		QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), util::toQstr("Ошибка в ui.xml"));
		exit(-1);
	}
	file.close();
}
void CMainWindow::initializeUI(){
	m_choiceComboBox.reset(new QComboBox(this));
	m_outputTextEdit.reset(new QTextEdit(this));
	m_submitButton.reset(new QPushButton(this));
	m_outputTextEdit->setReadOnly(true);
	auto node = m_doc.documentElement().firstChild();
	while (!node.isNull()) {
		m_choiceComboBox->addItem(node.toElement().attribute("caption"));
		node = node.nextSibling();
	}
	this->comboboxChanged(0);
}
void CMainWindow::rebuildUI(int index){
	m_widgetVec.clear();

	auto node = m_doc.documentElement().firstChild();
	for (int i = 0; i < index; ++i) {
		node = node.nextSibling();
	}

	for (node = node.firstChild(); !node.isNull(); node = node.nextSibling()){
		QString attrName = node.toElement().attribute("type");
		if (attrName == m_typeHidden) {
			m_widgetVec.push_back(WidgetPtr(new CHidden(this, &node)));
		}
		if (attrName == m_typeSimpleCheckbox) {
			m_widgetVec.push_back(WidgetPtr(new CSimpleCheckbox(this, &node)));
		}
		if (attrName == m_typeDoubleCheckbox) {
			m_widgetVec.push_back(WidgetPtr(new CDoubleCheckbox(this, &node)));
		}
		if (attrName == m_typeLinebox) {
			m_widgetVec.push_back(WidgetPtr(new CLineCheckbox(this, &node)));
		}
		if (attrName == m_typeSimpleLine) {
			m_widgetVec.push_back(WidgetPtr(new CSimpleLine(this, &node)));
		}
		if (attrName == m_typeDate) {
			m_widgetVec.push_back(WidgetPtr(new CDate(this, &node)));
		}
	}
	QResizeEvent evt(this->size(), this->size());
	this->resizeEvent(&evt);
}