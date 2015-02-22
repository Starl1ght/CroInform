#include <util.h>
#include <QtSQL>

class CSqlLayer : public QObject{
	Q_OBJECT
public:
	CSqlLayer() = default;
	~CSqlLayer();

	bool connect();
	void fetchApiCredentials(QString* login, QString* password);

public slots:
	void validateUserCredentials(QString, QString);

signals:
	void authSignal(authResult);
private:
	QSqlDatabase db;
};