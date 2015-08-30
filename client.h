#ifndef CLIENT_H
#define CLIENT_H

#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QString>

class Host;
class EditorBase;
class EditorFactoryBase;
class Session;

class Client
{
    public:
        Client();

        void loadConfig(const QJsonObject& config);

    private:
        void createHosts(const QJsonObject& hosts);
        void createEditors(const QJsonObject& editors);
        void createSessions(const QJsonArray& sessions);

        QMap<QString, Host*> m_hosts;
        QMap<QString, EditorBase*> m_editors;
        QMap<QString, Session*> m_sessions;

        QMap<QString, EditorFactoryBase*> m_editorFactories;
};

#endif // CLIENT_H
