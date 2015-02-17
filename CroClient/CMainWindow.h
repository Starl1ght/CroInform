#pragma once
#pragma warning (push)
#pragma warning (disable:4127) 
#include <QtWidgets>
#include <QtXML>
#pragma warning (pop)
#include "CAPIManager.h"
#include "CAuthDialog.h"
#include "CParser.h"

#include "ICustomWidget.h"
#include "WLineCheckbox.h"
#include "WSimpleCheckbox.h"
#include "WDoubleCheckbox.h"
#include "WDate.h"
#include "WHidden.h"
#include "WSimpleLine.h"

#include <memory>
#include <vector>

using WidgetPtr = std::unique_ptr<ICustomWidget>;
using WidgetVec = std::vector<WidgetPtr>;

class CMainWindow : public QMainWindow {
	Q_OBJECT
public:
	CMainWindow();
	~CMainWindow() = default;

	void resizeEvent(QResizeEvent * event) override;
	void closeEvent(QCloseEvent *event) override;
signals:
	void submitRequest(QString);

public slots:
	void prepareRequest(bool);
	void comboboxChanged(int);
	void showWindow();
	
	void unblockUI();
	void getFormattedData(QString);
private:
	void rebuildUI(int index);
	void initializeXML();
	void initializeUI();
	
	std::unique_ptr<QPushButton> m_submitButton;
	std::unique_ptr<QComboBox> m_choiceComboBox;
	std::unique_ptr<QTextEdit> m_outputTextEdit;
	
	QDomDocument m_doc;
	WidgetVec m_widgetVec;

	const int m_defHeight{ 700 };
	const int m_defWidth{ 800 };

	const QString m_typeDoubleCheckbox{ "DoubleCheckbox" };
	const QString m_typeLinebox{ "LineBox" };
	const QString m_typeHidden{ "Hidden" };
	const QString m_typeSimpleCheckbox{ "SimpleCheckbox" };
	const QString m_typeSimpleLine{ "SimpleLine" };
	const QString m_typeDate{ "Date" };
};
