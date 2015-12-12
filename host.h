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
        explicit Host(const QString& name, const QString& host, unsigned short port, unsigned int reconnectTime, QObject *parent = Q_NULLPTR);

        void addListener(const QString& name, const std::shared_ptr<ListenerBase>& listener);
        bool isConnected();

        void send(const Message& message);

        void connectToRemote();

        const QString& getName() const {
            return m_name;
        }

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
        bool m_connectStarted;
        QMap<QString, QList<std::shared_ptr<ListenerBase>>> m_listeners;
        QList<Message> m_messageQueue;
        QTimer* m_timer;
        QString m_id;
        unsigned int m_reconnectTime;
};

#endif // HOST_H
