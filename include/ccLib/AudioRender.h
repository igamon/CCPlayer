#ifndef AUDIORENDER_H
#define AUDIORENDER_H

#include "Thread.h"
#include "Common.h"
#include "SpinLock.h"
#include "ALWrapper.h"
#include "IMessageReceiver.h"

namespace CCPlayer
{

class CCAudioRender: public CCThread, public IMessageReceiver
{
public:
    CCAudioRender();
    virtual ~CCAudioRender();

public:
    void SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam);
    void ReceiverMessage(const SmartPtr<Event>& rSmtEvent);
    bool PopFrontMessage(SmartPtr<Event>& rSmtEvent);

public:
    virtual void Run();

private:
    int GetAudioCtx(AVFormatContext *pFormatCtx, int asIndex, ALenum* pChannels, ALuint *pRate, ALenum *pFormat, ALenum *pType);

private:
    std::queue<SmartPtr<Event> > m_messageQueue;
    CCSpinLock m_spinLockMessageQueue;
};

}

#endif // AUDIORENDER_H
