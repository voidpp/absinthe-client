#ifndef DIRECTFILEOPENERBASE_H
#define DIRECTFILEOPENERBASE_H

#include <QString>
#include <QStringList>

class DirectFileOpenerBase
{
    public:
        virtual void openFile(const QString& session, const QStringList& pathList) = 0;
};

#endif // DIRECTFILEOPENERBASE_H
