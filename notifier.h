#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QString>

class Notifier
{
    public:
        virtual void sendMessage(const QString& message, const QString& title = "Absinthe client") = 0;

};

#endif // NOTIFIER_H

