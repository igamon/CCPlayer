#include "Player.h"
#include "IRSPCommand.h"
#include "MessageCenter.h"
#include "ModuleManager.h"
#include "Any.h"
#include "SystemAlarm.h"

namespace CCPlayer
{

CCPlayer::CCPlayer()
:m_pRspCommentObject(NULL)
{
    //this will start the message center thread
    CCMessageCenter::GetInstance()->InitMessageCenter();
    CCMessageCenter::GetInstance()->RegisterMessageReceiver(MESSAGE_OBJECT_ENUM_PLAYER, this);

    //�����Լ����߳�
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
    CCMessageCenter::GetInstance()->PostMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                                MESSAGE_OBJECT_ENUM_VIDEO_RENDER,
                                                MESSAGE_TYPE_ENUM_INIT_GLRENDER_OBJECT,
                                                Any(pGLRenderView));
}

void CCPlayer::Open(const std::string& loadParams)
{
    CCMessageCenter::GetInstance()->PostMessage(MESSAGE_OBJECT_ENUM_CLIENT,
                                                MESSAGE_OBJECT_ENUM_PLAYER,
                                                COMMAND_TYPE_ENUM_OPEN,
                                                Any(loadParams));
}

void CCPlayer::Pause()
{
    CCMessageCenter::GetInstance()->PostMessage(MESSAGE_OBJECT_ENUM_CLIENT,
                                                MESSAGE_OBJECT_ENUM_PLAYER,
                                                COMMAND_TYPE_ENUM_PAUSE,
                                                Any());
}

void CCPlayer::Continue()
{
    CCMessageCenter::GetInstance()->PostMessage(MESSAGE_OBJECT_ENUM_CLIENT,
                                                MESSAGE_OBJECT_ENUM_PLAYER,
                                                COMMAND_TYPE_ENUM_CONTINUE,
                                                Any());
}

void CCPlayer::Stop()
{
    CCMessageCenter::GetInstance()->PostMessage(MESSAGE_OBJECT_ENUM_CLIENT,
                                                MESSAGE_OBJECT_ENUM_PLAYER,
                                                COMMAND_TYPE_ENUM_STOP,
                                                Any());
}

void CCPlayer::PostMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam)
{
    CCMessageCenter::GetInstance()->PostMessage(messageSender,
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
    CCSystemAlarm::GetInstance()->RegisterSystemAlarm(this);
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

                    PostMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                MESSAGE_OBJECT_ENUM_DATA_MANAGER,
                                MESSAGE_TYPE_ENUM_OPEN_FILE,
                                Any(mediaUrl));
                }
                break;
                case COMMAND_TYPE_ENUM_CONTINUE:
                {
                    CCMessageCenter::GetInstance()->PostMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                                                MESSAGE_OBJECT_ENUM_AUDIO_RENDER,
                                                                MESSAGE_TYPE_ENUM_AUDIO_CONTINUE,
                                                                Any());

                    CCMessageCenter::GetInstance()->PostMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                                                MESSAGE_OBJECT_ENUM_VIDEO_RENDER,
                                                                MESSAGE_TYPE_ENUM_VIDEO_CONTINUE,
                                                                Any());
                }
                break;
                case COMMAND_TYPE_ENUM_PAUSE:
                {
                        CCMessageCenter::GetInstance()->PostMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                                                    MESSAGE_OBJECT_ENUM_AUDIO_RENDER,
                                                                    MESSAGE_TYPE_ENUM_AUDIO_PAUSE,
                                                                    Any());

                        CCMessageCenter::GetInstance()->PostMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                                                    MESSAGE_OBJECT_ENUM_VIDEO_RENDER,
                                                                    MESSAGE_TYPE_ENUM_VIDEO_PAUSE,
                                                                    Any());
                }
                break;
                case COMMAND_TYPE_ENUM_STOP:
                {
                    PostMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                MESSAGE_OJBECT_ENUM_ALL,
                                MESSAGE_TYPE_ENUM_CLIENT_STOP,
                                Any());
                }
                break;
                case MESSAGE_TYPE_ENUM_OPENED_FILE:
                {
                    std::vector<Any> openedParams = any_cast<std::vector<Any> >(event.GetPtr()->anyParams);
                    int ret = any_cast<int>(openedParams[0]);
                    AVFormatContext* pAVFormatCtx = any_cast<AVFormatContext*>(openedParams[1]);
                    int asIndex = any_cast<int>(openedParams[2]);
                    int vsIndex = any_cast<int>(openedParams[3]);

                    if(asIndex != -1)
                    {
                        CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_AUDIO_DECODER);

                        std::vector<Any> audioStreamInfo;
                        audioStreamInfo.push_back(Any(pAVFormatCtx));
                        audioStreamInfo.push_back(Any(asIndex));

                        PostMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                    MESSAGE_OBJECT_ENUM_AUDIO_DECODER,
                                    MESSAGE_TYPE_ENUM_FINDED_AUDIO_STREAM,
                                    Any(audioStreamInfo));

                        CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_AUDIO_RENDER);
                    }

                    if(vsIndex != -1)
                    {
                        CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_VIDEO_DECODER);

                        std::vector<Any> videoStreamInfo;
                        videoStreamInfo.push_back(Any(pAVFormatCtx));
                        videoStreamInfo.push_back(Any(vsIndex));

                        PostMessage(MESSAGE_OBJECT_ENUM_PLAYER,
                                    MESSAGE_OBJECT_ENUM_VIDEO_DECODER,
                                    MESSAGE_TYPE_ENUM_FINDED_VIDEO_STREAM,
                                    Any(videoStreamInfo));

                        CCModuleManager::AddModule(MESSAGE_OBJECT_ENUM_VIDEO_RENDER);
                    }

                    if(m_pRspCommentObject != NULL)
                    {
                        m_pRspCommentObject->OpenResponse(ret);
                    }
                }
                break;
                case MESSAGE_TYPE_ENUM_DATA_MANAGER_EOF:
                {
                    m_bRunning = false;
                    continue;
                }
                break;
            }
        }

        CCFrequencyWorker::Wait();
        CCFrequencyWorker::Wait();
        CCFrequencyWorker::Wait();
        CCFrequencyWorker::Wait();
        CCFrequencyWorker::Wait();
        CCFrequencyWorker::Wait();

        //std::cout << "The play is running" << std::endl;
    }

    CCSystemAlarm::GetInstance()->UnRegisterSystemAlarm(this);
}

}
