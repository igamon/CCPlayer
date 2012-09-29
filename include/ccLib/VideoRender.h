#ifndef VIDEORENDER_H
#define VIDEORENDER_H

#include "Thread.h"
#include "Common.h"
#include "SpinLock.h"
#include "IMessageReceiver.h"

namespace CCPlayer
{

class CCVideoRender: public CCThread, public IMessageReceiver
{
public:
    CCVideoRender();
    virtual ~CCVideoRender();
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
    std::queue<SmartPtr<Event> > m_messageQueue;
    CCSpinLock m_spinLockMessageQueue;
};

}

#endif // VIDEORENDER_H
