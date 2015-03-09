#include "CSqlLayer.h"


CSqlLayer::~CSqlLayer(){
	db.close();
}

bool CSqlLayer::connect(){
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("croserver");
	db.setUserName("croserver");
	db.setPassword("1qazXSW@3edc");
	return db.open();
}

QString CSqlLayer::getURL(){
	QSqlQuery query;
	query.exec("SELECT `options`.`value` FROM `options` WHERE `options`.`key` = \"cro_url\"");
	query.next();
	return query.value(0).toString();
}

void CSqlLayer::fetchApiCredentials(QString* login, QString* password){
	QSqlQuery query;
	
	query.exec("SELECT `options`.`value` FROM `options` WHERE `options`.`key` = \"cro_login\"");
	query.next();
	*login = query.value(0).toString();
	
	query.exec("SELECT `options`.`value` FROM `options` WHERE `options`.`key` = \"cro_password\"");
	query.next();
	*password = query.value(0).toString();

	//std::cout << "Fetched credentials " << login->toStdString() << " " << password->toStdString() << std::endl;
}

void CSqlLayer::validateUserCredentials(QString login, QString password){
	QSqlQuery query;

	QString str{ "SELECT `permission` FROM `accounts` WHERE `login` = \"" };
	str += login;
	str += "\" AND `password` = \"";
	str += password;
	str += "\"";

	query.exec(str);
	query.next();

	if (query.isNull(0)) {
		emit authSignal(authResult::fail);
		return;
	}

	if (query.value(0).toInt() == 0) {
		emit authSignal(authResult::admin);
	} else {
		emit authSignal(authResult::user);
	}
}