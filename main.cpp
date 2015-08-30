#include "mainwindow.h"
#include <QApplication>

#include <QDebug>

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time yyyy-MM-dd HH:mm:ss.zzz} %{type} %{threadid} %{file}(%{line}): %{message}");
    QApplication app(argc, argv);
    app.setOrganizationName("QtProject");
    app.setApplicationName("Application Example");

    MainWindow win;

    qApp->setQuitOnLastWindowClosed(false);

    return app.exec();
}
