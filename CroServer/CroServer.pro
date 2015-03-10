#-------------------------------------------------
#
# Project created by QtCreator 2015-03-09T12:15:07
#
#-------------------------------------------------

QT       += core xml sql
QT       -= gui

TARGET = CroServer
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$PWD/../shared/ $$PWD/../asio/

LIBS += -L/opt/cprocsp/lib/amd64 -lcpcurl
#LIBS += -lcurl

SOURCES += \
    CServer.cpp \
    CSqlLayer.cpp \
    CWorker.cpp \
    CPlatformApiLayer.cpp

HEADERS += \
    CServer.h \
    CSqlLayer.h \
    CWorker.h \
    CPlatformApiLayer.h
