#pragma once
#include <util.h>
#include <mutex>
// qt don't like this one :D
#ifdef _WIN32
#import <msxml6.dll>

class CPlatformApiLayer {
public:
	CPlatformApiLayer(const QString & login, const QString & pass, const QString & url);
	~CPlatformApiLayer();
	CPlatformApiLayer& operator=(const CPlatformApiLayer&) = delete;
	CPlatformApiLayer(const CPlatformApiLayer&) = delete;
	bool auth();

	void requestApi(const QString & post, QString & out);
	void queryApi(const QString & id, QString & out);


private:
	QString m_workingDir{ "" };
	QString m_url{ "" };
	QString m_login{ "" };
	QString m_pass{ "" };
	MSXML2::IXMLHTTPRequestPtr m_xmlRequest;

	std::mutex m_mutex;
};

#endif



#ifdef __linux__ 
#define CPlatformApiLayer CPlatformApiLayer_L
#endif

