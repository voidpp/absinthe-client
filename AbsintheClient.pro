#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T15:56:35
#
#-------------------------------------------------

QT += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AbsintheClient
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    message.cpp \
    session.cpp \
    host.cpp \
    editorbase.cpp \
    listenerbase.cpp \
    client.cpp \
    nppeditor.cpp

HEADERS  += mainwindow.h \
    message.h \
    session.h \
    host.h \
    editorbase.h \
    listenerbase.h \
    client.h \
    nppeditor.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
