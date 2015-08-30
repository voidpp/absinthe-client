#include "host.h"

#include <QDebug>

Host::Host(const QString &name, const QString &host, unsigned short port, QObject *parent)
    :QObject(parent)
    ,m_name(name)
    ,m_connected(false)
    ,m_timer(new QTimer(this))
{
    connect(&m_webSocket, &QWebSocket::connected, this, &Host::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Host::onClose);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &Host::onTextMessageReceived);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onReconnectTimer()));

    m_timer->setSingleShot(true);

    m_url = QString("ws://%1:%2").arg(host, QString::number(port));

    m_id = m_url.toDisplayString() + " (" + m_name + ")";
}

void Host::connectToRemote()
{
    qDebug() << "Try to connect to" << m_id;
    m_webSocket.open(m_url);
}

bool Host::isConnected()
{
    return m_connected;
}

void Host::send(const Message &message)
{
    if(!isConnected()) {
        qDebug() << "Try to send message, but no open socket (towards" << m_id << "). Message:" << message;
        m_messageQueue.append(message);
        return;
    }

    qDebug() << "Send message to" << m_id << ". Message:" << message;

    m_webSocket.sendTextMessage(message);
}

void Host::onConnected()
{
    qDebug() << "Connected to" << m_id;

    m_connected = true;

    for(auto name: m_listeners.keys()) {
        for(auto listener: m_listeners.value(name))
            listener->onConnect();
    }

    for(auto&& message: m_messageQueue) {
        send(message);
    }
    m_messageQueue.clear();
}

void Host::onTextMessageReceived(const QString &message)
{
    Message msg(message);

    if(!msg.isValid()) {
        qDebug() << "Malformed message received from" << m_id << ". Message: " << message;
        return;
    }

    if (m_listeners.find(msg.getName()) == m_listeners.end()) {
        qDebug() << "There is no subscriber for this message:" << msg;
        return;
    }

    qDebug() << "Received message:" << msg;

    for(auto listener: m_listeners.value(msg.getName()))
        listener->onMessage(msg);
}

void Host::onClose()
{
    if(isConnected()) {
        qDebug() << "Socket closed on" << m_id;
        m_connected = false;
    }

    m_timer->start(1000);
}

void Host::onReconnectTimer()
{
    connectToRemote();
}

void Host::addListener(const QString &name, ListenerBase *listener)
{
    m_listeners[name].append(listener);
}
