#ifndef SESSION_H
#define SESSION_H

#include <QString>

#include "listenerbase.h"

class Host;
class EditorBase;

class Session: public ListenerBase
{
    public:
        Session(const QString& name, Host *host, EditorBase *editor, const QString& path, const QString& remotePath);

        virtual void onConnect();
        virtual void onMessage(const Message& message);

    private:
        QString m_name;
        Host* m_host;
        EditorBase* m_editor;

        QString m_path;
        QString m_remotePath;

};

#endif // SESSION_H
