#ifndef LISTENERBASE_H
#define LISTENERBASE_H

#include "message.h"

class ListenerBase
{
    public:
        virtual void onConnect() = 0;
        virtual void onMessage(const Message& message) = 0;
};

#endif // LISTENERBASE_H
