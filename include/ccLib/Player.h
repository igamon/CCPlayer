#ifndef CCPLAER_H
#define CCPLAER_H

#include "Common.h"
#include "Thread.h"
#include "IMessageReceiver.h"
#include "Event.h"
#include "SmartPtr.h"
#include "SpinLock.h"
#include "FrequencyWorker.h"

namespace CCPlayer
{

class IRSPCommand;
class CCUIGLView;
class CCUIWindow;

class CCPlayer : public CCThread, public IMessageReceiver, public CCFrequencyWorker
{
public:
    CCPlayer();
    virtual ~CCPlayer();

public:
    void SetRspCommandObject(IRSPCommand* pRspCommentObject);

public:
    void InitGLWindow(CCUIGLView* pGLRenderView);
    void Open(const std::string& loadParams);
    void Pause();
    void Continue();
    void Stop();

public:
    //get total display time
    int GetTotalDurationBySecond(int64_t* pTotalDuration);
    int GetCurrentPostionBySecond(int64_t* pCurrPostion);

public:
    void PostMessage(MessageObjectId messageSender,
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
    AVFormatContext* m_pAVFormatCtx;

private:
    IRSPCommand* m_pRspCommentObject;
};

}

#endif // CCPlayer_H
