#include "CPlatformApiLayer.h"


CPlatformApiLayer::CPlatformApiLayer(const QString & login, const QString & pass, const QString & url){
#ifdef _WIN32
	CoInitialize(NULL);
	m_xmlRequest.CreateInstance("Msxml2.XMLHTTP.6.0");
#else
    //curl_global_init(CURL_GLOBAL_DEFAULT);
    //m_curl = curl_easy_init();
#endif
    m_login = login;
	m_pass = pass;
	m_url = url;
}
CPlatformApiLayer::~CPlatformApiLayer(){
    lowlevelApiCall("Type=Logout&WorkingDirectory=" + m_workingDir);
}

#ifndef _WIN32
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp){
    ((QString*)userp)->append((char*)(contents));
}
#endif

QString CPlatformApiLayer::lowlevelApiCall(const QString & request){
#ifdef _WIN32
    m_xmlRequest->open("POST", util::toRu(m_url).data(), false);
    m_xmlRequest->setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    m_xmlRequest->setRequestHeader("Content-length", _bstr_t(request.length()));
    m_xmlRequest->setRequestHeader("Connection", "close");
    m_xmlRequest->send(util::toRu(request).data());
    return util::toQstr((char*)m_xmlRequest->responseText);
#else
    /*
    QString reply{ "" };
    CURLcode res;
    curl_easy_setopt(m_curl, CURLOPT_URL, util::toRu(m_url).data());
    curl_easy_setopt(m_curl, CURLOPT_POST, 1);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0);
    //res = curl_easy_setopt(m_curl, CURLOPT_SSLENGINE, "gost");
    //if (res != CURLE_OK){
    //    std::cout << curl_easy_strerror(res) << 1 << std::endl;
    //}
    //res = curl_easy_setopt(m_curl, CURLOPT_SSLCERTTYPE, "CERT_SHA1_HASH_PROP_ID:CERT_SYSTEM_STORE_LOCAL_MACHINE:MY");
    res = curl_easy_setopt(m_curl, CURLOPT_SSLCERTTYPE, "CERT_SHA1_HASH_PROP_ID:CERT_SYSTEM_STORE_CURRENT_USER:MY");
    if (res != CURLE_OK){
        std::cout << curl_easy_strerror(res) << 2 << std::endl;
    }
    res = curl_easy_setopt(m_curl, CURLOPT_SSLCERT, "ea538d6944fe1bd65d9db4989e3296988e7b715a");
    if (res != CURLE_OK){
        std::cout << curl_easy_strerror(res) << 3 << std::endl;
    }
    curl_easy_setopt(m_curl, CURLOPT_SSL_CIPHER_LIST, "ALL");
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, util::toRu(request).data());
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &reply);
    //curl_slist* headers;
    //headers = curl_slist_append(headers, "Content-type: application/x-www-form-urlencoded");
    //headers = curl_slist_append(headers, util::toRu(QString("Content-length: ") + QString::number(request.length())));
    //headers = curl_slist_append(headers, "Connection: close");
    //curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(m_curl);
    if (res != CURLE_OK){
        std::cout << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "bob4 -> " << util::toRu(reply).data() << std::endl;
    }

   // curl_slist_free_all(headers);
    return reply;
    */
    QString str{"/opt/cprocsp/bin/amd64/curl -k " + m_url  + " --data \""};
    str += request;
    str += "\"";
    FILE* f;
    f = popen(util::toRu(str).data(), "r");

    str.clear();
    char buf[4096];
    while (fgets(buf, 4096, f) != NULL){
        str.append(util::toQstr(buf));
    }
    return str;
#endif
}

bool CPlatformApiLayer::auth(){
    QString reply = lowlevelApiCall("Type=Login&Login=" + m_login + "&Password=" + m_pass);

    QDomDocument doc;
    if (!doc.setContent(reply)) {
        std::cout << "Reply verification failed. MSXML\\libCurl + CryptoPro fail." << std::endl;
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
    out = lowlevelApiCall("WorkingDirectory=" + m_workingDir + "&" + post);

	QDomDocument doc;
    doc.setContent(out);
	//if (doc.documentElement().firstChildElement("Error").text() == "")


	std::cout << "-- POST --" << std::endl;
    std::cout << util::toRu(post).toStdString() << std::endl;
    //std::cout << "-- REPLY --" << std::endl;
    //std::cout << util::toRu(out).toStdString() << std::endl;
}

void CPlatformApiLayer::queryApi(const QString & id, QString & out){
    out = lowlevelApiCall("Type=Answer&WorkingDirectory=" + m_workingDir + "&RequestNumber=" + id + "&=TypeAnswer=HV");

	std::cout << "-- QUERY -- " << std::endl;
	std::cout << util::toRu(out).toStdString() << std::endl;
}
