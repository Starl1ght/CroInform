#include "CPlatformApiLayer.h"

#ifdef _WIN32
CPlatformApiLayer::CPlatformApiLayer(const QString & login, const QString & pass, const QString & url){
	CoInitialize(NULL);
	m_xmlRequest.CreateInstance("Msxml2.XMLHTTP.6.0");
	m_login = login;
	m_pass = pass;
	m_url = url;
}
CPlatformApiLayer::~CPlatformApiLayer(){
	QString post{ "Type=Logout&WorkingDirectory=" };
	post += m_workingDir;
	m_xmlRequest->open("POST", util::toRu(m_url).data(), false);
	m_xmlRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	m_xmlRequest->setRequestHeader("Content-length", _bstr_t(post.length()));
	m_xmlRequest->setRequestHeader("Connection", "close");
	m_xmlRequest->send(util::toRu(m_url).data());
}

bool CPlatformApiLayer::auth(){
	m_xmlRequest->open("POST", util::toRu(m_url).data(), false);
	QString str{ "Type=Login&Login=" + m_login + "&Password=" + m_pass };
	m_xmlRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	m_xmlRequest->setRequestHeader("Content-length", _bstr_t(str.length()));
	m_xmlRequest->setRequestHeader("Connection", "close");
	m_xmlRequest->send(util::toRu(str).data());
	
	QDomDocument doc;
	if (!doc.setContent(util::toQstr((char*)m_xmlRequest->responseText))) {
		return false;
	}
	if (doc.documentElement().firstChildElement("WorkingDirectory").text().length() == 0) {
		std::cout << doc.toByteArray().toStdString();
		return false;
	}

	m_workingDir = doc.documentElement().firstChildElement("WorkingDirectory").text();
	std::cout << m_workingDir.toStdString();
	return true;
}

void CPlatformApiLayer::requestApi(const QString & post, QString & out){
	m_xmlRequest->open("POST", util::toRu(m_url).data(), false);
	QString str{ "WorkingDirectory=" + m_workingDir + "&" + post };
	m_xmlRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	m_xmlRequest->setRequestHeader("Content-length", _bstr_t(str.length()));
	m_xmlRequest->setRequestHeader("Connection", "close");
	m_xmlRequest->send(util::toRu(str).data());
	
	out = util::toQstr((char*)m_xmlRequest->responseText);

	QDomDocument doc;
	doc.setContent(out);
	//if (doc.documentElement().firstChildElement("Error").text() == "")


	std::cout << "-- POST --" << std::endl;
	std::cout << util::toRu(str).toStdString() << std::endl;
	std::cout << "-- REPLY --" << std::endl;
	std::cout << util::toRu(out).toStdString() << std::endl;
}

void CPlatformApiLayer::queryApi(const QString & id, QString & out){
	m_xmlRequest->open("POST", util::toRu(m_url).data(), false);
	QString str{ "Type=Answer&WorkingDirectory=" + m_workingDir + "&RequestNumber=" + id + "&=TypeAnswer=HV" };
	m_xmlRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	m_xmlRequest->setRequestHeader("Content-length", _bstr_t(str.length()));
	m_xmlRequest->setRequestHeader("Connection", "close");
	m_xmlRequest->send(util::toRu(str).data());

	out = util::toQstr((char*)m_xmlRequest->responseText);

	std::cout << "-- QUERY -- " << std::endl;
	std::cout << util::toRu(out).toStdString() << std::endl;
}
#endif



