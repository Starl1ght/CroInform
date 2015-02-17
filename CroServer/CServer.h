#include <thread>
#include <memory>
#include "asio.h"
#include "CPlatformApiLayer.h"
#include "CSqlLayer.h"
#include "CWorker.h"

class CServer {
public:
	CServer();
	~CServer() = default;
	CServer(const CServer&) = delete;
	CServer& operator=(const CServer&) = delete;
		
private:
	CPlatformApiLayer m_api;
	CSqlLayer m_sql;
	QString m_workingDir;
};