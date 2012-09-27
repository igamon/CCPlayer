#include "MessageCenter.h"
#include "IMessageReceiver.h"
#include "SmartPtr.h"

namespace CCPlayer
{

CCMessageCenter* CCMessageCenter::m_pSharedInstance = NULL;
CCSpinLock CCMessageCenter::m_spinLockInstance;

CCMessageCenter* CCMessageCenter::GetInstance()
{
    m_spinLockInstance.Lock();
    if(m_pSharedInstance == NULL)
    {
        m_pSharedInstance = new CCMessageCenter();
    }
    m_spinLockInstance.UnLock();
    return m_pSharedInstance;
}

void CCMessageCenter::RegisterMessageReceiver(MessageObjectId receiverObjectId, IMessageReceiver* pMessageReceiver)
{
    m_rwLockMessageReceiver.WriteLock();
    m_mapMessageReceiver.insert(std::map<MessageObjectId, IMessageReceiver*>::value_type(receiverObjectId, pMessageReceiver));
    m_rwLockMessageReceiver.UnLock();
}

void CCMessageCenter::UnRegisterMessageReceiver(MessageObjectId receiverObjectId)
{
    m_rwLockMessageReceiver.WriteLock();
    m_mapMessageReceiver.erase(receiverObjectId);
    m_rwLockMessageReceiver.UnLock();
}

IMessageReceiver* CCMessageCenter::GetMessageReceiver(MessageObjectId messageObjectId)
{
    IMessageReceiver* pMessageObject = NULL;
    m_rwLockMessageReceiver.ReadLock();
    std::map<MessageObjectId, IMessageReceiver*>::const_iterator pos = m_mapMessageReceiver.find(messageObjectId);
    if(pos != m_mapMessageReceiver.end())
    {
        pMessageObject = pos->second;
    }
    m_rwLockMessageReceiver.UnLock();
    return pMessageObject;
}

void CCMessageCenter::SendMessage(MessageObjectId messageSender,
                                  MessageObjectId messageReceiver,
                                  MessageType msg,
                                  Any anyParam)
{
    CCMessageCenter* pMessageCenter = CCMessageCenter::GetInstance();
    pMessageCenter->PushBackMessage(messageSender, messageReceiver, msg, anyParam);
}

void CCMessageCenter::PushBackMessage(MessageObjectId messageSender,
                         MessageObjectId messageReceiver,
                         MessageType msg,
                         Any anyParam)
{
    m_spinLockMessageQueue.Lock();
    Event event;
    event.pSendModule = GetMessageReceiver(messageSender);
    event.pReceiveModule = GetMessageReceiver(messageReceiver);
    event.type = msg;
    event.anyParams = anyParam;
    m_messageQueue.push(SmartPtr<Event>(new Event(event)));
    m_spinLockMessageQueue.UnLock();
}

bool CCMessageCenter::PopFrontMessage(SmartPtr<Event>& rSmtEvent)
{
    bool bGetMsg = false;
    m_spinLockMessageQueue.Lock();
    if(!m_messageQueue.empty())
    {
        rSmtEvent = m_messageQueue.front();
        m_messageQueue.pop();
        bGetMsg = true;
    }
    m_spinLockMessageQueue.UnLock();
    return bGetMsg;
}

void CCMessageCenter::InitMessageCenter()
{
    m_pSharedInstance->Launch();
}

void CCMessageCenter::Run()
{
    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            if(event.GetPtr()->pReceiveModule != NULL)
            {
                event.GetPtr()->pReceiveModule->ReceiverMessage(event);
            }
        }
        Sleep(10);
    }
}

}
