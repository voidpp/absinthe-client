#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QSystemTrayIcon>
#include <QDebug>

#include "client.h"
#include "searchdialog.h"

#include "qxtglobalshortcut.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_client(new Client(this, this))
{
    m_ui->setupUi(this);

    QDir path(QCoreApplication::applicationDirPath());
    m_config.setFilename(path.absoluteFilePath(QStringLiteral("config.json")));

    createActions();
    createTrayIcon();

    loadConfig();
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::reloadConfig()
{
    loadConfig();
}

void MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setText("Client application for Absinthe");
    msgBox.setInformativeText("A tiny app to open sshfs connected files from ssh console in any local editor. Connect to Absinthe server only.");
    msgBox.exec();
}

void MainWindow::onHotkey()
{
    qDebug() << "Open search dialog due global hotkey event received.";
    openSearchDialog();
}

void MainWindow::onOpenSearch()
{
    openSearchDialog();
}

void MainWindow::sendMessage(const QString& message, const QString& title)
{
    m_trayIcon->showMessage(title, message);
}

void MainWindow::openSearchDialog()
{
    SearchDialog dialog(m_config, m_client);
    dialog.exec();
}

bool MainWindow::loadConfig()
{
    if(!m_config.load()) {
        sendMessage("Cannot load config file");
        return false;
    }

    m_client->loadConfig(m_config.getData());

    bool searchEnabled = !m_config.getSessionsWithSearch().empty();

    if(searchEnabled) {
        QxtGlobalShortcut *shortcut = new QxtGlobalShortcut(this);
        shortcut->setShortcut(QKeySequence(m_config.getData()["search"].toObject()["hotkey"].toString()));
        connect(shortcut, &QxtGlobalShortcut::activated, this, &MainWindow::onHotkey);
    }

    m_showSearchDialogAction->setEnabled(searchEnabled);

    return true;
}

void MainWindow::createTrayIcon()
{
    m_trayIconMenu = new QMenu(this);

    m_trayIconMenu->addAction(m_reloadConfigAction);
    m_trayIconMenu->addAction(m_aboutAction);
    m_trayIconMenu->addAction(m_showSearchDialogAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/logo16.png"));
    m_trayIcon->setToolTip("Absinthe client");
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setVisible(true);
}

void MainWindow::createActions()
{
    m_quitAction = new QAction(tr("&Exit"), this);
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_reloadConfigAction = new QAction(tr("&Reload config"), this);
    connect(m_reloadConfigAction, SIGNAL(triggered()), this, SLOT(reloadConfig()));

    m_aboutAction = new QAction(tr("&About"), this);
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    m_showSearchDialogAction = new QAction(tr("&File search"), this);
    connect(m_showSearchDialogAction, SIGNAL(triggered()), this, SLOT(onOpenSearch()));
}

