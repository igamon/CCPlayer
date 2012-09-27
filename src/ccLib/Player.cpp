#include "Player.h"
#include "IGLRenderObject.h"
#include "MessageCenter.h"
#include "ModuleManager.h"
#include "Any.h"

namespace CCPlayer
{

CCPlayer::CCPlayer()
{
    //this will start the message center thread
    CCMessageCenter::GetInstance()->InitMessageCenter();
    CCMessageCenter::GetInstance()->RegisterMessageReceiver(MESSAGE_OBJECT_ENUM_CCPLAYER, this);

    //启动自己的线程
    Launch();
}

CCPlayer::~CCPlayer()
{
    CCMessageCenter::GetInstance()->UnRegisterMessageReceiver(MESSAGE_OBJECT_ENUM_CCPLAYER);
}

void CCPlayer::Open(const std::string& loadParams)
{
    CCMessageCenter::GetInstance()->SendMessage(MESSAGE_OBJECT_ENUM_OUTPLACE, MESSAGE_OBJECT_ENUM_CCPLAYER, MESSAGE_TYPE_ENUM_OPEN, Any(loadParams));
}

void CCPlayer::SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam)
{
    CCMessageCenter::GetInstance()->SendMessage(messageSender, messageReceiver, msg, anyParam);
}

void CCPlayer::ReceiverMessage(const SmartPtr<Event>& rSmtEvent)
{
    m_spinLockMessageQueue.Lock();
    m_messageQueue.push(rSmtEvent);
    m_spinLockMessageQueue.UnLock();
}

bool CCPlayer::PopFrontMessage(SmartPtr<Event>& rSmtEvent)
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

void CCPlayer::Run()
{
    //m_pGLRender->CreateGLContext();
    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            switch(event.GetPtr()->type)
            {
                case MESSAGE_TYPE_ENUM_OPEN:
                    {
                        CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_DATA_MANAGER);

                        SendMessage(MESSAGE_OBJECT_ENUM_CCPLAYER, MESSAGE_OBJECT_ENUM_DATA_MANAGER, event.GetPtr()->type, event.GetPtr()->anyParams);
                    }
                    break;
            }
            //std::cout << any_cast<std::string>(event.GetPtr()->anyParams) << std::endl;
        }

        Sleep(10);

    //    m_pGLRender->DrawFrame();
    //    m_pGLRender->SwapBuffers();
    //    Sleep(10);
    }
}

}
