#ifndef AUDIODECODER_H
#define AUDIODECODER_H

#include "Thread.h"

namespace CCPlayer
{

class CCAudioDecoder : public CCThread
{
public:
    CCAudioDecoder();
    virtual ~CCAudioDecoder();
};

}

#endif // AUDIODECODER_H
