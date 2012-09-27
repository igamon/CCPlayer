#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <cassert>
#include <ctime>

#include <Windows.h>

#define SUCCESS     0
#define FAILURE     -1

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}

namespace CCPlayer
{

enum MessageObjectId
{
    MESSAGE_OJBECT_ENUM_ALL,
    MESSAGE_OBJECT_ENUM_VIDEO_RENDER,
    MESSAGE_OBJECT_ENUM_AUDIO_RENDER,
    MESSAGE_OBJECT_ENUM_VIDEO_DECODER,
    MESSAGE_OBJECT_ENUM_AUDIO_DECODER,
    MESSAGE_OBJECT_ENUM_DATA_MANAGER,
    MESSAGE_OBJECT_ENUM_CCPLAYER,
    MESSAGE_OBJECT_ENUM_OUTPLACE,   //Íâ²¿Ä£¿é
    MESSAGE_OBJECT_MAX
};

struct FFmpegStuff
{
    AVFormatContext *pAVFormatContext;
};

}

#endif
