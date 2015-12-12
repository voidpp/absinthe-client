#ifndef VSCODEEDITOR_H
#define VSCODEEDITOR_H

#include "editorbase.h"

class VsCodeEditor : public EditorBase
{
    public:
        VsCodeEditor(const QString& name, const QString& binPath);

        virtual QStringList getCommandArguments(const QString &filename, unsigned int line);
        virtual void setFocus();
        virtual bool openContent(const QString&);
};

#endif // VSCODEEDITOR_H
