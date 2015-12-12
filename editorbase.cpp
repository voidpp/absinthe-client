#include "editorbase.h"

#include <QDebug>
#include <QProcess>

EditorBase::EditorBase(const QString &name, const QString& binPath)
    :m_name(name)
    ,m_binPath(binPath)
{
}

bool EditorBase::openFile(const QString &filename, unsigned int line)
{
    qDebug() << "Open file:" << filename;

    QStringList args = getCommandArguments(filename, line);

    qDebug() << "Editor:" << m_binPath << ", args:" << args;

    QProcess process;
    process.setProgram(m_binPath);
    process.setArguments(args);
    process.start();

    process.waitForFinished();

    qDebug() << "File open result:" << process.exitCode();

    return true;
}
