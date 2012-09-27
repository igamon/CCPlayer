#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include "Thread.h"

namespace CCPlayer
{

class CCVideoDecoder : public CCThread
{
public:
    CCVideoDecoder();
    virtual ~CCVideoDecoder();
};

}

#endif // VIDEODECODER_H
