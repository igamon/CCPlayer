#ifndef CCPLAER_H
#define CCPLAER_H

#include "Common.h"
#include "Thread.h"
#include "IMessageReceiver.h"
#include "Event.h"
#include "SmartPtr.h"
#include "SpinLock.h"

class IGLRenderObject;

namespace CCPlayer
{

class CCPlayer : public CCThread, public IMessageReceiver
{
public:
    CCPlayer();
    virtual ~CCPlayer();

public:
    void Open(const std::string& loadParams);

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

#endif // CCPlayer_H
