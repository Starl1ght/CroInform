#pragma once
#pragma warning (push)
#pragma warning (disable:4127)
#include <QtWidgets>
#include <QtXML>
#pragma warning (pop)
#include <memory>

class ICustomWidget : public QObject {
public:
	ICustomWidget() = default;
	ICustomWidget(const ICustomWidget&) = delete;
	virtual ~ICustomWidget() = default;
	virtual QString getPostData() = 0;
	virtual void reposition(int x, int y) = 0;
	virtual int getYforNextWidget() = 0;
};