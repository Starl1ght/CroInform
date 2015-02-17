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

void CSqlLayer::fetchApiCredentials(QString* login, QString* password){
	QSqlQuery query;
	
	query.exec("SELECT `options`.`value` FROM `options` WHERE `options`.`key` = \"cro_login\"");
	query.next();
	*login = query.value(0).toString();
	
	query.exec("SELECT `options`.`value` FROM `options` WHERE `options`.`key` = \"cro_password\"");
	query.next();
	*password = query.value(0).toString();
}

authResult CSqlLayer::validateUserCredentials(const QString & login, const QString & password){
	QSqlQuery query;

	QString str{ "SELECT `permission` FROM `accounts` WHERE `login` = \"" };
	str += login;
	str += "\" AND `password` = \"";
	str += password;
	str += "\"";

	query.exec(str);
	query.next();

	if (query.isNull(0)) {
		return authResult::fail;
	}

	switch (query.value(0).toInt()) {
	case 0:
		return authResult::admin;
		break;
	case 1:
		return authResult::user;
		break;
	default:
		std::cout << "Boys, we have DB inconsistency here";
		exit(-14);
	}
}