#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QtWebSockets/QWebSocket>

#include "listenerbase.h"
#include "message.h"

class Host: public QObject
{
    Q_OBJECT

    public:
        explicit Host(const QString& name, const QString& host, unsigned short port, QObject *parent = Q_NULLPTR);

        void addListener(const QString& name, ListenerBase* listener);
        bool isConnected();

        void send(const Message& message);

        void connectToRemote();

    private Q_SLOTS:

        void onConnected();
        void onTextMessageReceived(const QString& message);
        void onClose();
        void onReconnectTimer();

    private:

        QUrl m_url;
        QWebSocket m_webSocket;
        QString m_name;
        bool m_connected;
        QMap<QString, QList<ListenerBase*>> m_listeners;
        QList<Message> m_messageQueue;
        QTimer* m_timer;
        QString m_id;
};

#endif // HOST_H
