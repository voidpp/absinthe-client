#include "nppeditor.h"

NppEditor::NppEditor(const QString &name, const QString& binPath)
    :EditorBase(name, binPath)
{

}

QStringList NppEditor::getCommandArguments(const QString &filename, unsigned int line)
{
    return QStringList() << filename << "-n" + QString::number(line);
}

void NppEditor::setFocus()
{

}

bool NppEditor::openContent(const QString &content)
{
    return false;
}
