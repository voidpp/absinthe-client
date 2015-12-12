#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QEvent>
#include <QTimer>
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QKeyEvent>
#include <QtWebSockets/QWebSocket>

#include "config.h"
#include "directfileopenerbase.h"

class SearchDialog;

class KeyPressEater : public QObject
{
    Q_OBJECT
    public:
        KeyPressEater(SearchDialog * dlg) :m_dialog(dlg) {}

    protected:
        bool eventFilter(QObject *obj, QEvent *event);
        SearchDialog* m_dialog;
};

class SearchDialog : public QDialog
{
    Q_OBJECT

    public:
        SearchDialog(Config& config, DirectFileOpenerBase* fileOpener);
        virtual ~SearchDialog();

        bool onKeyEvent(QKeyEvent* keyEvent);

    private Q_SLOTS:
        void onSearch(const QString&);
        void onSession(const QString& text);
        void onFileSelected(QListWidgetItem*);

        void onConnected();
        void onTextMessageReceived(const QString& message);
        void onClose();
        void onSearchDelayTimer();

    private:
        QJsonObject findSession(const QString& name);

        QLineEdit *m_fileInput;
        QComboBox *m_projectsComboBox;
        QComboBox *m_sessionsComboBox;
        QListWidget *m_filesList;
        QWebSocket m_webSocket;
        QTimer* m_timer;

        Config& m_config;
        DirectFileOpenerBase* m_fileOpener;
};

#endif // SEARCHDIALOG_H
