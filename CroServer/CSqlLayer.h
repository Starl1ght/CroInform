#include <util.h>
#include <QtSql>

class CSqlLayer : public QObject{
	Q_OBJECT
public:
	CSqlLayer() = default;
	~CSqlLayer();

	bool connect();
	void fetchApiCredentials(QString* login, QString* password);
	QString getURL();
public slots:
	void validateUserCredentials(QString, QString);

signals:
	void authSignal(authResult);
private:
	QSqlDatabase db;
};
