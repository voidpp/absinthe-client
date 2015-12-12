#include "client.h"

#include <QSystemTrayIcon>

#include "notifier.h"
#include "session.h"
#include "host.h"
#include "editorbase.h"
#include "nppeditor.h"
#include "vscodeeditor.h"

Client::Client(Notifier *notifier, QWidget *widget)
{
    m_widget = widget;
    m_notifier = notifier;
    m_editorFactories["npp"] = std::make_shared<EditorFactory<NppEditor>>();
    m_editorFactories["vscode"] = std::make_shared<EditorFactory<VsCodeEditor>>();
    m_self = std::shared_ptr<Client>(this);
}

void Client::loadConfig(const QJsonObject &config)
{
    qDebug() << "Load client config";

    for(auto&& it : m_sessions) {
        it.second.reset();
    }

    for(auto&& it : m_editors) {
        it.second.reset();
    }

    for(auto&& it : m_hosts) {
        it.second.reset();
    }

    createHosts(config["hosts"].toObject());
    createEditors(config["editors"].toObject());
    createSessions(config["sessions"].toArray());
}

void Client::onConnect(Host &host)
{
    //m_notifier->sendMessage(QString("Connected to host '%1'").arg(host.getName()));
}

void Client::onClose(Host &host)
{
    //m_notifier->sendMessage(QString("Disconnected from host '%1'").arg(host.getName()));
}

void Client::openFile(const QString &session, const QStringList &pathList)
{
    if(m_sessions.find(session) == m_sessions.end()){
        return;
    }

    std::shared_ptr<Session>& sess = m_sessions[session];
    for (auto it : pathList)
        sess->openFile(it.split("/"));
    sess->setFocus();
}

void Client::createHosts(const QJsonObject &hosts)
{
    for(auto name : hosts.keys()) {
        const QJsonObject& value = hosts.value(name).toObject();
        unsigned int reconnect = value.find("reconnect") == value.end() ? 10000 : value["reconnect"].toInt();
        auto host = std::make_shared<Host>(name, value["host"].toString(), value["port"].toInt(), reconnect);
        m_hosts[name] = host;
        host->addListener("", m_self);
    }
}

void Client::createEditors(const QJsonObject &editors)
{
    for(auto name : editors.keys()) {
        auto factoryIt = m_editorFactories.find(name);
        if(factoryIt == m_editorFactories.end()) {
            qDebug() << "Undefined editor:" << name;
            continue;
        }

        m_editors[name] = factoryIt->second->create(name, editors.value(name).toString());
    }
}

void Client::createSessions(const QJsonArray &sessions)
{
    for(auto&& session: sessions) {
        QJsonObject sess = session.toObject();
        if(sess["enabled"] == false)
            continue;

        auto hostIt = m_hosts.find(sess["host"].toString());
        if(hostIt == m_hosts.end()) {
            qDebug() << "Undefined host" << sess["host"] << "in session" << sess["name"] << ". Ignoring session.";
            continue;
        }

        auto editorIt = m_editors.find(sess["editor"].toString());
        if(editorIt == m_editors.end()) {
            qDebug() << "Undefined editor" << sess["editor"] << "in session" << sess["name"] << ". Ignoring session.";;
            continue;
        }

        auto sessonInstance = std::make_shared<Session>(sess["name"].toString(), editorIt->second, sess["path"].toString(), sess["remote-path"].toString(), m_widget);

        hostIt->second->addListener(sess["name"].toString(), sessonInstance);

        hostIt->second->connectToRemote();

        m_sessions[sess["name"].toString()] = sessonInstance;
    }
}
