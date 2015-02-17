#pragma once
#include <QDialog>
#include <QtWidgets>
#include <memory>
class CAuthDialog : public QDialog {
	Q_OBJECT
public:
	CAuthDialog();
	~CAuthDialog() = default;

	void closeEvent(QCloseEvent * e);
public slots:
	void authResult(bool);
	void buttonClicked(bool);

signals:
	void authSignal(QString, QString);
	void authSuccessful();
	
private:
	std::unique_ptr<QLabel> m_loginLabel;
	std::unique_ptr<QLabel> m_passLabel;
	std::unique_ptr<QLineEdit> m_loginLine;
	std::unique_ptr<QLineEdit> m_passLine;
	std::unique_ptr<QPushButton> m_loginButton;

	QString m_login;
	QString m_pass;
};