#include "CAPIManager.h"

CAPIManager::CAPIManager(){

}

void CAPIManager::postReceiver(QString post){
	asio::error_code ec;
	
	QDomDocument doc;
	QDomElement root = doc.createElement("post");
	root.setAttribute("data", post);
	doc.appendChild(root);

	QByteArray arr = util::toRu(doc.toString());
	ec = util::asio_write(m_socket, arr); // posted POST request;
	ec = util::asio_read(m_socket, arr); // reply


	QString answer = util::toQstr(arr);
	doc.setContent(answer);
	
	root = doc.documentElement().firstChildElement();
	if (root.tagName() == "Error" || root.tagName() == "Warning") {
		emit forwardErrorToParse(answer);
		return;
	}

	if (root.tagName() == "RequestNumber") {
		queryRequest(root.text());
	}
}

void CAPIManager::authSlot(QString login, QString password){
	QDomDocument doc;
	QFile file("connect.xml");
	if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file)) {
		QMessageBox::information(nullptr, QString::fromLocal8Bit("Ошибка"), util::toQstr("Файл connect.xml не найден"));
		exit(-13421);
	}

	QDomElement root = doc.documentElement();
	try {
		m_socket.reset(new tcp::socket(io));
		if (root.attribute("mode") == "ip") {
			auto ip = asio::ip::address_v4::from_string(root.attribute("ip").toStdString().c_str());
			auto port = static_cast<unsigned short>(root.attribute("port").toInt());
			tcp::endpoint ep{ ip, port };
			m_socket->connect(ep);
		} else if (root.attribute("mode") == "domain") {
			tcp::resolver res(io);
			tcp::resolver::query query(
				root.attribute("domain").toStdString().c_str(),
				root.attribute("port").toStdString().c_str()
				);
			auto ep = *res.resolve(query);
			m_socket->connect(ep);
		}
	} catch (std::exception &e) {
		e; // warning
		emit authResultSignal(authResult::serverError); 
		return;
	}
		
	doc.clear();
	QByteArray arr;
	root = doc.createElement("auth");
	root.setAttribute("login", login);
	root.setAttribute("password", password);
	doc.appendChild(root);

	arr = util::toRu(doc.toString());
		
	asio::error_code ec;
	
	ec = util::asio_write(m_socket, arr);
	ec = util::asio_read(m_socket, arr);

	doc.setContent(arr);
	if (doc.documentElement().tagName() == "auth") {
		if (doc.documentElement().attribute("result") == "fail") {
			emit authResultSignal(authResult::fail);
		} 
		if (doc.documentElement().attribute("result") == "admin") {
			emit authResultSignal(authResult::admin);
		}
		if (doc.documentElement().attribute("result") == "user") {
			emit authResultSignal(authResult::user);
		}
	}
}

void CAPIManager::queryRequest(QString id) {
	asio::error_code ec;

	QByteArray arr{ "<query id=\"" + util::toRu(id) + "\"/>" };
	ec = util::asio_write(m_socket, arr);
	ec = util::asio_read(m_socket, arr);

	emit forwardAnswerToParse(util::toQstr(arr));
}

void CAPIManager::querySlot(QString id){
	queryRequest(id);
}