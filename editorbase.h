#ifndef EDITORBASE_H
#define EDITORBASE_H

#include <memory>
#include <QString>
#include <QStringList>

class EditorBase
{
    public:
        EditorBase(const QString& name, const QString& binPath);
        virtual ~EditorBase() {}

        virtual QStringList getCommandArguments(const QString &filename, unsigned int line) = 0;
        virtual void setFocus() = 0;
        virtual bool openContent(const QString& content) = 0;

        bool openFile(const QString& filename, unsigned int line);

    protected:
        QString m_name;
        QString m_binPath;
};

class EditorFactoryBase
{
    public:
        virtual std::shared_ptr<EditorBase> create(const QString& name, const QString& binPath) = 0;
};

template<class T>
class EditorFactory: public EditorFactoryBase
{
    public:
        std::shared_ptr<T> create(const QString& name, const QString& binPath) {
            return std::make_shared<T>(name, binPath);
        }
};

#endif // EDITORBASE_H
