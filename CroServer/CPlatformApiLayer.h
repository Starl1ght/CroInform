#include <util.h>

// qt don't like this one :D
#ifdef _WIN32
#import <msxml6.dll>

class CPlatformApiLayer {
public:
	CPlatformApiLayer();
	~CPlatformApiLayer();
	CPlatformApiLayer& operator=(const CPlatformApiLayer&) = delete;
	CPlatformApiLayer(const CPlatformApiLayer&) = delete;
	bool auth(const QString & login, const QString & pass);
private:
	QString m_workingDir{ "" };
	const QString m_url{ "https://ssl.croinform.ru:450/api.test" };
	MSXML2::IXMLHTTPRequestPtr m_xmlRequest;
};

#endif



#ifdef __linux__ 
#define CPlatformApiLayer CPlatformApiLayer_L
#endif

