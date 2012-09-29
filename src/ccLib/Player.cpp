#include "Player.h"
#include "IGLRenderObject.h"
#include "IRSPPlayerCommand.h"
#include "MessageCenter.h"
#include "ModuleManager.h"
#include "Any.h"

namespace CCPlayer
{

CCPlayer::CCPlayer()
{
    //this will start the message center thread
    CCMessageCenter::GetInstance()->InitMessageCenter();
    CCMessageCenter::GetInstance()->RegisterMessageReceiver(MESSAGE_OBJECT_ENUM_PLAYER, this);

    //启动自己的线程
    Launch();
}

CCPlayer::~CCPlayer()
{
    CCMessageCenter::GetInstance()->UnRegisterMessageReceiver(MESSAGE_OBJECT_ENUM_PLAYER);
}

void CCPlayer::Open(const std::string& loadParams)
{
    CCMessageCenter::GetInstance()->SendMessage(MESSAGE_OBJECT_ENUM_CLIENT, MESSAGE_OBJECT_ENUM_PLAYER, COMMAND_TYPE_ENUM_OPEN, Any(loadParams));
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
    std::string mediaUrl;
    IRSPPlayCommand* pRspObject = NULL;

    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            switch(event.GetPtr()->type)
            {
                case COMMAND_TYPE_ENUM_OPEN:
                    {
                        mediaUrl = any_cast<std::string>(event.GetPtr()->anyParams);

                        //Create the media source pump to support the data packet
                        CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_DATA_MANAGER);

                        SendMessage(MESSAGE_OBJECT_ENUM_PLAYER, MESSAGE_OBJECT_ENUM_DATA_MANAGER, MESSAGE_TYPE_ENUM_OPEN_FILE, Any(mediaUrl));
                        //CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_AUDIO_RENDER);
                        //CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_AUDIO_DECODER);
                        //CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_VIDEO_RENDER);
                        //CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_VIDEO_DECODER);

                        //SendMessage(MESSAGE_OBJECT_ENUM_CCPLAYER, MESSAGE_OBJECT_ENUM_DATA_MANAGER, event.GetPtr()->type, event.GetPtr()->anyParams);
                        //SendMessage(MESSAGE_OBJECT_ENUM_CCPLAYER, MESSAGE_OBJECT_ENUM_AUDIO_RENDER, event.GetPtr()->type, event.GetPtr()->anyParams);
                        //SendMessage(MESSAGE_OBJECT_ENUM_CCPLAYER, MESSAGE_OBJECT_ENUM_AUDIO_DECODER, event.GetPtr()->type, event.GetPtr()->anyParams);
                        //SendMessage(MESSAGE_OBJECT_ENUM_CCPLAYER, MESSAGE_OBJECT_ENUM_VIDEO_RENDER, event.GetPtr()->type, event.GetPtr()->anyParams);
                        //SendMessage(MESSAGE_OBJECT_ENUM_CCPLAYER, MESSAGE_OBJECT_ENUM_VIDEO_DECODER, event.GetPtr()->type, event.GetPtr()->anyParams);
                    }
                    break;
                case MESSAGE_TYPE_ENUM_OPENED_FILE:
                    {
                        std::vector<Any> openedParams = any_cast<std::vector<Any> >(event.GetPtr()->anyParams);
                        int ret = any_cast<int>(openedParams[0]);
                        AVFormatContext* pAVFormatCtx = any_cast<AVFormatContext*>(openedParams[1]);
                        int vsIndex = any_cast<int>(openedParams[2]);
                        int asIndex = any_cast<int>(openedParams[3]);

                        if(asIndex != -1)
                        {
                            std::vector<Any> findedASParams;
                            findedASParams.push_back(Any(asIndex));
                            findedASParams.push_back(Any(pAVFormatCtx));

                            CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_AUDIO_DECODER);
                            SendMessage(MESSAGE_OBJECT_ENUM_PLAYER, MESSAGE_OBJECT_ENUM_AUDIO_DECODER, MESSAGE_TYPE_ENUM_FINDED_AUDIO_STREAM, findedASParams);

                            CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_AUDIO_RENDER);
                            SendMessage(MESSAGE_OBJECT_ENUM_PLAYER, MESSAGE_OBJECT_ENUM_AUDIO_RENDER, MESSAGE_TYPE_ENUM_FINDED_AUDIO_STREAM, findedASParams);
                        }

                        if(pRspObject != NULL)
                        {
                            pRspObject->OpenResponse(ret);
                        }
                    }
                    break;
            }
            //std::cout << "CCPlayer" << any_cast<std::string>(event.GetPtr()->anyParams) << std::endl;
        }

        Sleep(10);
    }
}

}
