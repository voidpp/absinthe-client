#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

#include "filelogger.h"

#include "windows.h"

int main(int argc, char *argv[])
{
    //FileLogger logger("absinthe.log");
    //qInstallMessageHandler(FileLogger::message);

    AllowSetForegroundWindow(ASFW_ANY);

    QApplication app(argc, argv);
    app.setOrganizationName("QtProject");
    app.setApplicationName("Application Example");

    MainWindow win;

    qApp->setQuitOnLastWindowClosed(false);

    return app.exec();
}
