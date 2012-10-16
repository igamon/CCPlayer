#include "Player.h"
#include "IRSPCommand.h"
#include "MessageCenter.h"
#include "ModuleManager.h"
#include "Any.h"

namespace CCPlayer
{

CCPlayer::CCPlayer()
:m_pRspCommentObject(NULL)
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

void CCPlayer::SetRspCommandObject(IRSPCommand* pRspCommentObject)
{
    m_pRspCommentObject = pRspCommentObject;
}

void CCPlayer::InitGLWindow(CCUIGLView* pGLRenderView)
{
    CCMessageCenter::GetInstance()->SendMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                                MESSAGE_OBJECT_ENUM_VIDEO_RENDER,
                                                MESSAGE_TYPE_ENUM_INIT_GLRENDER_OBJECT,
                                                Any(pGLRenderView));
}

void CCPlayer::Open(const std::string& loadParams)
{
    CCMessageCenter::GetInstance()->SendMessage(MESSAGE_OBJECT_ENUM_CLIENT,
                                                MESSAGE_OBJECT_ENUM_PLAYER,
                                                COMMAND_TYPE_ENUM_OPEN,
                                                Any(loadParams));
}

void CCPlayer::SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam)
{
    CCMessageCenter::GetInstance()->SendMessage(messageSender,
                                                messageReceiver,
                                                msg,
                                                anyParam);
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
    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            switch(event.GetPtr()->type)
            {
                case COMMAND_TYPE_ENUM_OPEN:
                    {
                        std::string mediaUrl = any_cast<std::string>(event.GetPtr()->anyParams);

                        CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_DATA_MANAGER);

                        SendMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                    MESSAGE_OBJECT_ENUM_DATA_MANAGER,
                                    MESSAGE_TYPE_ENUM_OPEN_FILE,
                                    Any(mediaUrl));
                    }
                    break;
                case MESSAGE_TYPE_ENUM_OPENED_FILE:
                    {
                        std::vector<Any> openedParams = any_cast<std::vector<Any> >(event.GetPtr()->anyParams);
                        int ret = any_cast<int>(openedParams[0]);
                        //AVFormatContext* pAVFormatCtx = any_cast<AVFormatContext*>(openedParams[1]);
                        AVCodecContext* pAudioCtx = any_cast<AVCodecContext*>(openedParams[2]);
                        AVCodecContext* pVideoCtx = any_cast<AVCodecContext*>(openedParams[3]);

                        if(pAudioCtx != NULL)
                        {
                            CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_AUDIO_DECODER);
                            SendMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                        MESSAGE_OBJECT_ENUM_AUDIO_DECODER,
                                        MESSAGE_TYPE_ENUM_FINDED_AUDIO_STREAM,
                                        Any(pAudioCtx));

                            CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_AUDIO_RENDER);
                        }

                        if(pVideoCtx != NULL)
                        {
                            CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_VIDEO_DECODER);
                            SendMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                        MESSAGE_OBJECT_ENUM_VIDEO_DECODER,
                                        MESSAGE_TYPE_ENUM_FINDED_VIDEO_STREAM,
                                        Any(pVideoCtx));

                            CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_VIDEO_RENDER);
                        }

                        if(m_pRspCommentObject != NULL)
                        {
                            m_pRspCommentObject->OpenResponse(ret);
                        }
                    }
                    break;
            }
        }
        Sleep(10);
    }
}

}
