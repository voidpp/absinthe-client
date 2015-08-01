#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    createActions();
    createTrayIcon();

    loadConfig();
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

void MainWindow::showMessage(const QString& message, const QString& title)
{
    m_trayIcon->showMessage(title, message);
}

bool MainWindow::loadConfig()
{
    QDir path(QCoreApplication::applicationDirPath());

    QFile loadFile(path.absoluteFilePath(QStringLiteral("config.json")));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        showMessage("Couldn't open config file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    m_config = loadDoc.object();

    return true;
}

void MainWindow::createTrayIcon()
{
    m_trayIconMenu = new QMenu(this);

    m_trayIconMenu->addAction(m_reloadConfigAction);
    m_trayIconMenu->addAction(m_aboutAction);
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
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
