#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>

#include "client.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QSystemTrayIcon;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void reloadConfig();
        void about();

    private:
        void createTrayIcon();
        void createActions();
        bool loadConfig();
        void showMessage(const QString& message, const QString& title = "Absinthe Client message");

        Ui::MainWindow *m_ui;

        QSystemTrayIcon *m_trayIcon;

        QMenu *m_trayIconMenu;

        QAction *m_quitAction;
        QAction *m_reloadConfigAction;
        QAction *m_aboutAction;

        QJsonObject m_config;

        Client m_client;
};

#endif // MAINWINDOW_H
