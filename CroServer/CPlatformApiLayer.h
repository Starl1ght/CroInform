#pragma once
#include <util.h>
#include <mutex>
#ifdef _WIN32
#import <msxml6.dll>
#else
//#include <curl/curl.h>
#endif
class CPlatformApiLayer {
public:
	CPlatformApiLayer(const QString & login, const QString & pass, const QString & url);
	~CPlatformApiLayer();
	CPlatformApiLayer& operator=(const CPlatformApiLayer&) = delete;
	CPlatformApiLayer(const CPlatformApiLayer&) = delete;
	bool auth();

	void requestApi(const QString & post, QString & out);
	void queryApi(const QString & id, QString & out);

    QString lowlevelApiCall(const QString& request);

private:
	QString m_workingDir{ "" };
	QString m_url{ "" };
	QString m_login{ "" };
	QString m_pass{ "" };

#ifdef _WIN32
	MSXML2::IXMLHTTPRequestPtr m_xmlRequest;
#else
//   CURL *m_curl;
#endif


	std::mutex m_mutex;
};
