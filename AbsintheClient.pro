#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T15:56:35
#
#-------------------------------------------------

QT += core gui websockets widgets

TARGET = AbsintheClient
TEMPLATE = app

CONFIG += c++11

include(qxtglobalshortcut5/qxt.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    message.cpp \
    session.cpp \
    host.cpp \
    editorbase.cpp \
    client.cpp \
    nppeditor.cpp \
    filelogger.cpp \
    vscodeeditor.cpp \
    windowhelper.cpp \
    searchdialog.cpp \
    searchmessage.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    message.h \
    session.h \
    host.h \
    editorbase.h \
    listenerbase.h \
    client.h \
    nppeditor.h \
    notifier.h \
    filelogger.h \
    vscodeeditor.h \
    windowhelper.h \
    searchdialog.h \
    searchmessage.h \
    config.h \
    directfileopenerbase.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

LIBS += -luser32
