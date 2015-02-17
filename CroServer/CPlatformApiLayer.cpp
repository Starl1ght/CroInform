#include "CPlatformApiLayer.h"
#ifdef _WIN32

CPlatformApiLayer::CPlatformApiLayer(){
	CoInitialize(NULL);
	m_xmlRequest.CreateInstance("Msxml2.XMLHTTP.6.0");
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
bool CPlatformApiLayer::auth(const QString & login, const QString & pass){
	m_xmlRequest->open("POST", util::toRu(m_url).data(), false);
	QString str{ "Type=Login&Login=" + login + "&Password=" + pass };
	m_xmlRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	m_xmlRequest->setRequestHeader("Content-length", _bstr_t(str.length()));
	m_xmlRequest->setRequestHeader("Connection", "close");
	m_xmlRequest->send(util::toRu(str).data());
	
	QDomDocument doc;
	if (!doc.setContent(util::toQstr((char*)m_xmlRequest->responseText))) {
		return false;
	}
	if (doc.documentElement().firstChildElement("WorkingDirectory").text().length() == 0) {
		return false;
	}

	m_workingDir = doc.documentElement().firstChildElement("WorkingDirectory").text();
	return true;
}
#endif
