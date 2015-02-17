#include "CParser.h"

void CParser::parseAnswer(QString answer){
	emit parsed(answer);
	emit unblockUI();
}

void CParser::parseError(QString error){
	emit parsed(error);
	emit unblockUI();
}