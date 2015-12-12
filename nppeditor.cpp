#include "nppeditor.h"
#include "windowhelper.h"

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
    WindowHelper::SetFocus("Notepad++");
}

bool NppEditor::openContent(const QString&)
{
    return false;
}
