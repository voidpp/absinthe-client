#ifndef LISTENERBASE_H
#define LISTENERBASE_H

#include <memory>
#include "message.h"

class Host;

class ListenerBase
{
    public:
        virtual void onConnect(Host& host) = 0;
        virtual void onMessage(const Message& message, Host& host) = 0;
        virtual void onClose(Host& host) = 0;
};

#endif // LISTENERBASE_H
