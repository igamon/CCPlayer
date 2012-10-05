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
    AVCodecContext* pVideoCtx = NULL;
    SwsContext* pImageConvertCtx = NULL;
    int imgWidth = 0;
    int imgHeight = 0;

    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            switch(event.GetPtr()->type)
            {
                case MESSAGE_TYPE_ENUM_FINDED_VIDEO_STREAM:
                {
                    pVideoCtx = any_cast<AVCodecContext*>(event.GetPtr()->anyParams);

                    if(pVideoCtx != NULL)
                    {
                        GetVideoInformation(pVideoCtx, &pImageConvertCtx, &imgWidth, &imgHeight);

                        //send the width and height of the image frame to the render.
                        //incomplete.

                    }

                }
                break;
                case MESSAGE_TYPE_ENUM_GET_VIDEO_PACKET:
                {

                }
                break;
            } // end switch case
        }

        Sleep(10);
    }
}

int CCVideoDecoder::GetVideoInformation(AVCodecContext* pVideoCtx,
                                        SwsContext** ppImageConvertCtx,
                                        int *pImgWidth,
                                        int *pImgHeight)
{
    *pImgWidth = VIDEO_OUTPUT_WIDTH;
    *pImgHeight = VIDEO_OUTPUT_HEIGHT;

    *ppImageConvertCtx = sws_getContext(pVideoCtx->width,
                   pVideoCtx->height,
                   pVideoCtx->pix_fmt,
                   VIDEO_OUTPUT_WIDTH,
                   VIDEO_OUTPUT_HEIGHT,
                   PIX_FMT_RGBA,
                   SWS_BICUBIC,
                   NULL,
                   NULL,
                   NULL);
    return 0;
}

}
