#ifndef EVENT_H
#define EVENT_H

#include "Any.h"

namespace CCPlayer
{

class IMessageReceiver;

enum MessageType
{
//this is for client control the player
    COMMAND_TYPE_ENUM_OPEN,

//this is for player control the all modules
    MESSAGE_TYPE_ENUM_OPEN_FILE,
    MESSAGE_TYPE_ENUM_OPENED_FILE,

    MESSAGE_TYPE_ENUM_FINDED_AUDIO_STREAM,
    MESSAGE_TYPE_ENUM_FINDED_VIDEO_STREAM,

    MESSAGE_TYPE_ENUM_GET_AUDIO_PACKET,
    MESSAGE_TYPE_ENUM_GET_AUDIO_FRAME,
    MESSAGE_TYPE_ENUM_GET_VIDEO_PACKET,

    MESSAGE_TYPE_ENUM_GET_AUDIO_INFORMATION,
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
