#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>

#include "notifier.h"
#include "config.h"

#include "windows.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QSystemTrayIcon;
QT_END_NAMESPACE

class Client;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, public Notifier
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void reloadConfig();
        void about();
        void onHotkey();
        void onOpenSearch();

    private:
        void createTrayIcon();
        void createActions();
        bool loadConfig();        
        virtual void sendMessage(const QString& message, const QString& title = "Absinthe client") override;
        void openSearchDialog();

        Ui::MainWindow *m_ui;

        QSystemTrayIcon *m_trayIcon;

        QMenu *m_trayIconMenu;

        QAction *m_quitAction;
        QAction *m_reloadConfigAction;
        QAction *m_aboutAction;
        QAction *m_showSearchDialogAction;

        Config m_config;

        Client* m_client;
};

#endif // MAINWINDOW_H
