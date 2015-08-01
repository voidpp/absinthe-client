#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("QtProject");
    app.setApplicationName("Application Example");

    MainWindow win;
    //win.show();

    qApp->setQuitOnLastWindowClosed(false);

    return app.exec();
}
