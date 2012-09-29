#include "VideoDecoder.h"
#include "MessageCenter.h"

namespace CCPlayer
{

CCVideoDecoder::CCVideoDecoder()
{
}

CCVideoDecoder::~CCVideoDecoder()
{
}

void CCVideoDecoder::SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam)
{
    CCMessageCenter::GetInstance()->SendMessage(messageSender, messageReceiver, msg, anyParam);
}

void CCVideoDecoder::ReceiverMessage(const SmartPtr<Event>& rSmtEvent)
{
    m_spinLockMessageQueue.Lock();
    m_messageQueue.push(rSmtEvent);
    m_spinLockMessageQueue.UnLock();
}

bool CCVideoDecoder::PopFrontMessage(SmartPtr<Event>& rSmtEvent)
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

void CCVideoDecoder::Run()
{
    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            std::cout << "CCVideoDecoder" << any_cast<std::string>(event.GetPtr()->anyParams) << std::endl;
        }

        Sleep(10);
    }
}

}
