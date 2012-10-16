#include "VideoRender.h"
#include "MessageCenter.h"
#include "UIGLView.h"
#include "UIWindow.h"
#include "GLWrapper.h"
#include "GLViewImplWin32.h"

namespace CCPlayer
{

enum VideoRenderStatus
{
    VIDEO_RENDER_STATUS_ENUM_UNKNOW,
    VIDEO_RENDER_STATUS_ENUM_INITTED,
    VIDEO_RENDER_STATUS_ENUM_UPDATING,
    VIDEO_RENDER_STATUS_ENUM_SLEEPING,
    VIDEO_REDNER_STATUS_ENUM_DEADING,
    VIDEO_RENDER_STATUS_ENUM_MAX
};

CCVideoRender::CCVideoRender()
{
}

CCVideoRender::~CCVideoRender()
{
}

void CCVideoRender::SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam)
{
    CCMessageCenter::GetInstance()->SendMessage(messageSender, messageReceiver, msg, anyParam);
}

void CCVideoRender::ReceiverMessage(const SmartPtr<Event>& rSmtEvent)
{
    m_spinLockMessageQueue.Lock();
    m_messageQueue.push(rSmtEvent);
    m_spinLockMessageQueue.UnLock();
}

bool CCVideoRender::PopFrontMessage(SmartPtr<Event>& rSmtEvent)
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

void CCVideoRender::Run()
{
    CCGLWrapper glWrapper;
    int imgWidth = 0;
    int imgHeight = 0;

    CCGLViewImplWin32* pGLRenderHandle = NULL;

    VideoRenderStatus status
                        = VIDEO_RENDER_STATUS_ENUM_UNKNOW;

    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            switch(event.GetPtr()->type)
            {
                case MESSAGE_TYPE_ENUM_INIT_GLRENDER_OBJECT:
                {
                    pGLRenderHandle  = any_cast<CCGLViewImplWin32*>(event.GetPtr()->anyParams);

                    /*pGLRenderHandle = new CCUIGLView();
                    pGLRenderHandle->CreateRenderRect(
                                        pGLWindow,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT);
                    */
                    pGLRenderHandle->CreateGLContext();
                    status = VIDEO_RENDER_STATUS_ENUM_INITTED;
                }
                break;
                case MESSAGE_TYPE_ENUM_GET_VIDEO_INFORMATION:
                {
                    std::vector<Any> videoInformartion
                                        =any_cast<std::vector<Any> >(event.GetPtr()->anyParams);
                    imgWidth = any_cast<int>(videoInformartion[0]);
                    imgHeight = any_cast<int>(videoInformartion[1]);
                }
                break;
            } // end of case event
        } // end of get a message

        switch(status)
        {
            case VIDEO_RENDER_STATUS_ENUM_INITTED:
            {
                pGLRenderHandle->DrawFrame1();
            }
            break;
            case VIDEO_RENDER_STATUS_ENUM_UPDATING:
            {
                //pGLRenderObject->DrawFrame();
            }
            break;
            case VIDEO_RENDER_STATUS_ENUM_SLEEPING:
            {

            }
            break;
            case VIDEO_REDNER_STATUS_ENUM_DEADING:
            {

            }
            break;
        } // end of the render status

        Sleep(100);
    }
}

}
