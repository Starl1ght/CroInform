#include "CAPIManager.h"

CAPIManager::CAPIManager(){
	CoInitialize(NULL);
	m_XMLRequest.CreateInstance("Msxml2.XMLHTTP.6.0");
}

CAPIManager::~CAPIManager(){
	QString post{ "Type=Logout&WorkingDirectory=" };
	post += m_workingDir;
	m_XMLRequest->open("POST", m_URL.toLocal8Bit().constData(), false);
	m_XMLRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	m_XMLRequest->setRequestHeader("Content-length", _bstr_t(post.length()));
	m_XMLRequest->setRequestHeader("Connection", "close");
	m_XMLRequest->send(post.toLocal8Bit().constData());
	//m_XMLRequest->Release();
}

void CAPIManager::queryRequest(QString id){
	QString post{ "Type=Answer&TypeAnswer=HV&WorkingDirectory=" };
	post += m_workingDir;
	post += "&RequestNumber=";
	post += id;
		
	m_XMLRequest->open("POST", m_URL.toLocal8Bit().constData(), false);
	m_XMLRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	m_XMLRequest->setRequestHeader("Content-length", _bstr_t(post.length()));
	m_XMLRequest->setRequestHeader("Connection", "close");
	m_XMLRequest->send(post.toLocal8Bit().constData());

	emit forwardAnswerToParse(QString::fromLocal8Bit(m_XMLRequest->responseText));
}

void CAPIManager::postReceiver(QString post){
	post += "&WorkingDirectory=";
	post += m_workingDir;

	m_XMLRequest->open("POST", m_URL.toLocal8Bit().constData(), false);
	m_XMLRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	m_XMLRequest->setRequestHeader("Content-length", _bstr_t(post.length()));
	m_XMLRequest->setRequestHeader("Connection", "close");
	m_XMLRequest->send(post.toLocal8Bit().constData());

	QDomDocument doc;
	if (doc.setContent(QString::fromLocal8Bit(m_XMLRequest->responseText))) {
		if (doc.documentElement().firstChildElement("RequestNumber").text().length() != 0) {
			queryRequest(doc.documentElement().firstChildElement("RequestNumber").text());
		} else {
			emit forwardErrorToParse(doc.toString());
		}
	} else {
			MessageBox(NULL, L"You got shit, instead of reply from server. Strange.", L"ERROR", MB_OK);
		exit(-2);
	}
}

void CAPIManager::authSlot(QString login, QString password){
	m_XMLRequest->open("POST", m_URL.toLocal8Bit().constData(), false);

	QString str{ "Type=Login&Login=" + login + "&Password=" + password };
	m_XMLRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	m_XMLRequest->setRequestHeader("Content-length", _bstr_t(str.length()));
	m_XMLRequest->setRequestHeader("Connection", "close");
	m_XMLRequest->send(str.toLocal8Bit().constData());

	str = QString::fromLocal8Bit(m_XMLRequest->responseText);

	QDomDocument doc;
	if (doc.setContent(str) && doc.documentElement().firstChildElement("WorkingDirectory").text().length() != 0) {
		m_workingDir = doc.documentElement().firstChildElement("WorkingDirectory").text();
		emit authResult(true);
	} else {
		emit authResult(false);
	}
}