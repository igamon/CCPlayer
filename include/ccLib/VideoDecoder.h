#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include "Thread.h"
#include "Common.h"
#include "SpinLock.h"
#include "VideoDef.h"
#include "IMessageReceiver.h"

namespace CCPlayer
{

class CCVideoDecoder : public CCThread, public IMessageReceiver
{
public:
    CCVideoDecoder();
    virtual ~CCVideoDecoder();

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
    int GetVideoInformation(AVCodecContext* pVideoCtx,
                                        SwsContext** ppImageConvertCtx,
                                        int *pImgWidth,
                                        int *pImgHeight);

private:
    std::queue<SmartPtr<Event> > m_messageQueue;
    CCSpinLock m_spinLockMessageQueue;
};

}

#endif // VIDEODECODER_H
