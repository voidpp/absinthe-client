#ifndef NPPEDITOR_H
#define NPPEDITOR_H

#include "editorbase.h"

class NppEditor : public EditorBase
{
    public:
        NppEditor(const QString& name, const QString& binPath);

        virtual QStringList getCommandArguments(const QString &filename, unsigned int line);
        virtual void setFocus();
        virtual bool openContent(const QString& content);
};

#endif // NPPEDITOR_H
