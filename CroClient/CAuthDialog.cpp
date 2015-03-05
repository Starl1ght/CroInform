#include "CAuthDialog.h"

CAuthDialog::CAuthDialog(){
	m_loginLabel.reset(new QLabel(this));
	m_loginLine.reset(new QLineEdit(this));
	m_passLabel.reset(new QLabel(this));
	m_passLine.reset(new QLineEdit(this)); 
	m_loginButton.reset(new QPushButton(this));

	this->resize(220,200);
	Qt::WindowFlags flags(Qt::MSWindowsFixedSizeDialogHint & ~(Qt::WindowContextHelpButtonHint));
	this->setWindowFlags(flags);
	
	m_loginLabel->setGeometry(10, 10, 200, 20);
	m_loginLabel->setText(QString::fromLocal8Bit("Логин"));
	m_loginLabel->setAlignment(Qt::AlignCenter);

	m_loginLine->setGeometry(10, 35, 200, 25);
	m_loginLine->setAlignment(Qt::AlignCenter);

	m_passLabel->setGeometry(10, 70, 200, 20);
	m_passLabel->setText(QString::fromLocal8Bit("Пароль"));
	m_passLabel->setAlignment(Qt::AlignCenter);
	
	m_passLine->setGeometry(10, 100, 200, 25);
	m_passLine->setEchoMode(QLineEdit::Password);
	m_passLine->setAlignment(Qt::AlignCenter);

	m_loginButton->setGeometry(10, 140, 200, 40);
	m_loginButton->setText(QString::fromLocal8Bit("Логин"));

	QObject::connect(m_loginButton.get(), &QPushButton::clicked, this, &CAuthDialog::buttonClicked);
	this->show();
}

void CAuthDialog::closeEvent(QCloseEvent * e){
	e; // no warning 
	QApplication::exit();
}

void CAuthDialog::authResultSlot(authResult res) {
	switch (res) {
	case authResult::fail:
		m_loginButton->setText(util::toQstr("Неверно!"));
		break;
	case authResult::serverError:
		m_loginButton->setText(util::toQstr("Сервер не отвечает"));
		break;
	case authResult::admin:
	case authResult::user:
		emit authSuccessful(); // TODO - admin & user :)
		this->destroy();
	}
}



void CAuthDialog::buttonClicked(bool unused){
	unused; 
	emit authSignal(m_loginLine->text(), m_passLine->text());
}

