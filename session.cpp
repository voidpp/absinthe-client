#include "session.h"

#include "host.h"
#include "editorbase.h"

#include <QDir>

Session::Session(const QString& name, Host *host, EditorBase *editor, const QString& path, const QString& remotePath)
   :m_name(name)
   ,m_host(host)
   ,m_editor(editor)
   ,m_path(path)
   ,m_remotePath(remotePath)
{
    m_host->addListener(m_name, this);
}

void Session::onConnect()
{
    Message msg(m_name, "session_start");
    msg["remote_path"] = m_remotePath;

    m_host->send(msg);
}

void Session::onMessage(const Message &message)
{
    if (message.getCommand() == "open_file") {
        QString path = m_path + "/" + message["filename"].toStringList().join("/");
        m_editor->openFile(QDir::toNativeSeparators(path), message["line"].toInt());
    } else if (message.getCommand() == "set_focus") {
        m_editor->setFocus();
    }
}
