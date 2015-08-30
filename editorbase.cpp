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

    QProcess process;
    process.setProgram(m_binPath);
    process.setArguments(getCommandArguments(filename, line));
    process.start();

    process.waitForFinished();

    qDebug() << "File open result:" << process.exitCode();

    return true;
}
