#include "AudioDecoder.h"
#include "MessageCenter.h"

namespace CCPlayer
{

CCAudioDecoder::CCAudioDecoder()
{
}

CCAudioDecoder::~CCAudioDecoder()
{
}

void CCAudioDecoder::SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam)
{
    CCMessageCenter::GetInstance()->SendMessage(messageSender, messageReceiver, msg, anyParam);
}

void CCAudioDecoder::ReceiverMessage(const SmartPtr<Event>& rSmtEvent)
{
    m_spinLockMessageQueue.Lock();
    m_messageQueue.push(rSmtEvent);
    m_spinLockMessageQueue.UnLock();
}

bool CCAudioDecoder::PopFrontMessage(SmartPtr<Event>& rSmtEvent)
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

void CCAudioDecoder::Run()
{
    AVCodecContext *pAudioCodecCtx = NULL;
    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            switch(event.GetPtr()->type)
            {
                case MESSAGE_TYPE_ENUM_FINDED_AUDIO_STREAM:
                    {
                        std::vector<Any> findedASParams = any_cast<std::vector<Any> >(event.GetPtr()->anyParams);
                        int asIndex = any_cast<int>(findedASParams[0]);
                        AVFormatContext* pFormatCtx = any_cast<AVFormatContext*>(findedASParams[1]);

                        if(FindAudioDecoderContext(&pAudioCodecCtx, pFormatCtx, asIndex) == 0)
                        {
                            std::cout << "The Audio Decoder context is OK" << std::endl;
                        }else
                        {
                            std::cout << "The Audio Decoder context is BAD" << std::endl;
                        }
                    }
                    break;
                case MESSAGE_TYPE_ENUM_REQUEST_AUDIO_BYTES_FIRST_TIME:
                    {
                        std::vector<Any> reqFirstTime = any_cast<std::vector<Any> >(event.GetPtr()->anyParams);


                    }
                    break;
            }
        }

        Sleep(10);
    }
}

int CCAudioDecoder::FindAudioDecoderContext(AVCodecContext** ppASDecoderCtx, AVFormatContext* pFormatCtx, int asIndex)
{
    if(pFormatCtx == NULL || asIndex == -1)
    {
        return FAILURE;
    }

    *ppASDecoderCtx = pFormatCtx->streams[asIndex]->codec;

    return SUCCESS;
}

}
