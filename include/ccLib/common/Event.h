#ifndef EVENT_H
#define EVENT_H

#include "Any.h"

namespace CCPlayer
{

class IMessageReceiver;

enum MessageType
{
    MESSAGE_TYPE_ENUM_OPEN,
};

struct Event
{
    IMessageReceiver* pSendModule;
    IMessageReceiver* pReceiveModule;
    MessageType type;
    Any anyParams;
};

}

#endif
