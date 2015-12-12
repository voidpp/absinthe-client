#include "session.h"

#include "host.h"
#include "editorbase.h"

#include <QDir>

Session::Session(const QString& name, const std::shared_ptr<EditorBase>& editor, const QString& path, const QString& remotePath, QWidget *widget)
   :m_name(name)
   ,m_editor(editor)
   ,m_path(path)
   ,m_remotePath(remotePath)
   ,m_widget(widget)
{
}

void Session::onConnect(Host& host)
{
    Message msg(m_name, "session_start");
    msg["remote_path"] = m_remotePath;

    host.send(msg);
}

void Session::onMessage(const Message &message, Host&)
{
    m_widget->activateWindow();
    if (message.getCommand() == "open_file") {
        openFile(message["filename"].toStringList(), message["line"].toInt());
    } else if (message.getCommand() == "set_focus") {
        setFocus();
    }
}

void Session::openFile(const QStringList &pathlist, unsigned int line)
{
    QString path = m_path + "/" + pathlist.join("/");
    m_editor->openFile(QDir::toNativeSeparators(path), line);
}

void Session::setFocus()
{
    m_editor->setFocus();
}
