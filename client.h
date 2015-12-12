#ifndef CLIENT_H
#define CLIENT_H

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QWidget>

#include <map>
#include <memory>

#include "listenerbase.h"
#include "directfileopenerbase.h"

class Host;
class EditorBase;
class EditorFactoryBase;
class Session;
class Notifier;

class Client;

class Client: public ListenerBase, public DirectFileOpenerBase
{
    public:
        Client(Notifier* notifier, QWidget *widget);

        void loadConfig(const QJsonObject& config);

        virtual void onConnect(Host& host);
        virtual void onMessage(const Message&, Host&) {}
        virtual void onClose(Host& host);

        virtual void openFile(const QString& session, const QStringList& pathList) override;

    private:
        void createHosts(const QJsonObject& hosts);
        void createEditors(const QJsonObject& editors);
        void createSessions(const QJsonArray& sessions);

        std::map<QString, std::shared_ptr<Host>> m_hosts;
        std::map<QString, std::shared_ptr<EditorBase>> m_editors;
        std::map<QString, std::shared_ptr<Session>> m_sessions;

        std::map<QString, std::shared_ptr<EditorFactoryBase>> m_editorFactories;

        Notifier* m_notifier;

        std::shared_ptr<Client> m_self;

        QWidget *m_widget;
};

#endif // CLIENT_H
