#include "vscodeeditor.h"
#include "windowhelper.h"

VsCodeEditor::VsCodeEditor(const QString& name, const QString& binPath)
    :EditorBase(name, binPath)
{

}

QStringList VsCodeEditor::getCommandArguments(const QString &filename, unsigned int line)
{
    return QStringList() << "-g" << filename + ":" + QString::number(line) << "-r";
}

void VsCodeEditor::setFocus()
{
    WindowHelper::SetFocus("Visual Studio Code");
}

bool VsCodeEditor::openContent(const QString&)
{
    return false;
}
