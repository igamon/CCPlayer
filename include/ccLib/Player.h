#ifndef CCPLAER_H
#define CCPLAER_H

#include "Common.h"
#include "Thread.h"
#include "IMessageReceiver.h"
#include "Event.h"
#include "SmartPtr.h"
#include "SpinLock.h"

namespace CCPlayer
{

class IRSPCommand;
class CCGLViewImplWin32;
class CCUIWindow;

class CCPlayer : public CCThread, public IMessageReceiver
{
public:
    CCPlayer();
    virtual ~CCPlayer();

public:
    void SetRspCommandObject(IRSPCommand* pRspCommentObject);

public:
    void InitGLWindow(CCGLViewImplWin32* pGLWindow);
    void Open(const std::string& loadParams);

public:
    void SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam);
    void ReceiverMessage(const SmartPtr<Event>& rSmtEvent);
    bool PopFrontMessage(SmartPtr<Event>& rSmtEvent);

private:
    int FindAudioDecoderContext(AVCodecContext** ppASDecoderCtx, AVFormatContext* pFormatCtx, int asIndex);

public:
    virtual void Run();

private:
    std::queue<SmartPtr<Event> > m_messageQueue;
    CCSpinLock m_spinLockMessageQueue;

private:
    IRSPCommand* m_pRspCommentObject;
};

}

#endif // CCPlayer_H
