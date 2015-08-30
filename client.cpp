#include "client.h"

#include "session.h"
#include "host.h"
#include "editorbase.h"
#include "nppeditor.h"

Client::Client()
{
    m_editorFactories["npp"] = new EditorFactory<NppEditor>();
}

void Client::loadConfig(const QJsonObject &config)
{
    //TODO: remove all sessions, editors, hosts

    qDebug() << "Load client config";

    createHosts(config["hosts"].toObject());
    createEditors(config["editors"].toObject());
    createSessions(config["sessions"].toArray());
}

void Client::createHosts(const QJsonObject &hosts)
{
    for(auto name : hosts.keys()) {
        const QJsonObject& value = hosts.value(name).toObject();
        Host* host = new Host(name, value["host"].toString(), value["port"].toInt());
        m_hosts[name] = host;
        host->connectToRemote();
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

        m_editors[name] = factoryIt.value()->create(name, editors.value(name).toString());
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

        m_sessions[sess["name"].toString()] = new Session(sess["name"].toString(), hostIt.value(), editorIt.value(), sess["path"].toString(), sess["remote-path"].toString());
    }
}
