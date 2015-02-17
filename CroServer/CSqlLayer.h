#include <util.h>
#include <QtSQL>

class CSqlLayer {
public:
	CSqlLayer() = default;
	~CSqlLayer();

	bool connect();
	void fetchApiCredentials(QString* login, QString* password);
	authResult CSqlLayer::validateUserCredentials(const QString & login, const QString & password);
private:
	QSqlDatabase db;
};