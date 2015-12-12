#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QWidget>

#include "listenerbase.h"


class EditorBase;

class Session: public ListenerBase
{
    public:
        Session(const QString& name, const std::shared_ptr<EditorBase>& editor, const QString& path, const QString& remotePath, QWidget* widget);

        virtual void onConnect(Host& host);
        virtual void onMessage(const Message& message, Host&);
        virtual void onClose(Host&) {}

        void openFile(const QStringList &pathlist, unsigned int line = 0);
        void setFocus();

    private:
        QString m_name;        
        std::shared_ptr<EditorBase> m_editor;

        QString m_path;
        QString m_remotePath;
        QWidget* m_widget;
};

#endif // SESSION_H
