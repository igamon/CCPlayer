#include "AudioRender.h"
#include "MessageCenter.h"
#include "ALWrapper.h"

namespace CCPlayer
{

#define AUDIO_BUFFER_SIZE 192000
#define AUDIO_NUM_BUFFERS 6

CCAudioRender::CCAudioRender()
{
}

CCAudioRender::~CCAudioRender()
{
}

void CCAudioRender::SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam)
{
    CCMessageCenter::GetInstance()->SendMessage(messageSender, messageReceiver, msg, anyParam);
}

void CCAudioRender::ReceiverMessage(const SmartPtr<Event>& rSmtEvent)
{
    m_spinLockMessageQueue.Lock();
    m_messageQueue.push(rSmtEvent);
    m_spinLockMessageQueue.UnLock();
}

bool CCAudioRender::PopFrontMessage(SmartPtr<Event>& rSmtEvent)
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

void CCAudioRender::Run()
{
    ALWrapper alWrapper;

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

                    ALenum audType = 0;
                    ALuint audRate = 0;
                    ALenum audFormat = 0;
                    ALenum audChannels = 0;

                    if(GetAudioCtx(pFormatCtx, asIndex, &audChannels, &audRate, &audFormat, &audType) == 0)
                    {
                        alWrapper.SetAudioCtx(audChannels, audRate, audFormat, audType);

                        int bytePerBuffer = alWrapper.GetPerBufferBytes();
                        int buffers = alWrapper.GetAudioBuffers();

                        std::vector<Any> reqFirstTime;
                        reqFirstTime.push_back(Any(buffers));
                        reqFirstTime.push_back(Any(bytePerBuffer));

                        SendMessage(MESSAGE_OBJECT_ENUM_AUDIO_RENDER, MESSAGE_OBJECT_ENUM_AUDIO_DECODER, MESSAGE_TYPE_ENUM_REQUEST_AUDIO_BYTES_FIRST_TIME, Any(reqFirstTime));
                    }
                }
                break;
            }
        }

        Sleep(10);
    }
}

int CCAudioRender::GetAudioCtx(AVFormatContext *pFormatCtx, int asIndex, ALenum* pChannels, ALuint *pRate, ALenum *pFormat, ALenum *pType)
{
    AVCodecContext *audioCodecCtx = pFormatCtx->streams[asIndex]->codec;

    if (audioCodecCtx->channels < 1
            || audioCodecCtx->channels > 8
            || audioCodecCtx->channels == 3
            || audioCodecCtx->channels == 5)
    {
        return FAILURE;
    }
    *pChannels = audioCodecCtx->channels;
    *pRate = audioCodecCtx->sample_rate;
    if (audioCodecCtx->sample_fmt == AV_SAMPLE_FMT_U8)
    {
        *pType = AL_UNSIGNED_BYTE_SOFT;

        if (*pChannels == 1)
        {
            *pFormat = AL_FORMAT_MONO8;
        }
        else if (*pChannels == 2)
        {
            *pFormat = AL_FORMAT_STEREO8;
        }
        else if (alIsExtensionPresent("AL_EXT_MCFORMATS"))
        {
            if (*pChannels == 4)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_QUAD8");
            }
            else if (*pChannels == 6)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_51CHN8");
            }
            else if (*pChannels == 7)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_71CHN8");
            }
            else if (*pChannels == 8)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_81CHN8");
            }
        }
    }
    else if (audioCodecCtx->sample_fmt == AV_SAMPLE_FMT_S16)
    {
        *pType = AL_SHORT_SOFT;

         if (*pChannels == 1)
        {
            *pFormat = AL_FORMAT_MONO16;
        }
        else if (*pChannels == 2)
        {
            *pFormat = AL_FORMAT_STEREO16;
        }
        else if (alIsExtensionPresent("AL_EXT_MCFORMATS"))
        {
            if (*pChannels == 4)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_QUAD16");
            }
            else if (*pChannels == 6)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_51CHN16");
            }
            else if (*pChannels == 7)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_61CHN16");
            }
            else if (*pChannels == 8)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_71CHN16");
            }
        }
    }
    else if (audioCodecCtx->sample_fmt == AV_SAMPLE_FMT_DBL)
    {
        if (alIsExtensionPresent("AL_EXT_double"))
        {
            *pType = AL_DOUBLE_SOFT;

            if (*pChannels == 1)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_MONO_DOUBLE_EXT");
            }
            else if (*pChannels == 2)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_STEREO_DOUBLE_EXT");
            }
        }
    }
    else if ((audioCodecCtx->sample_fmt == AV_SAMPLE_FMT_S32
            && sizeof(int32_t) == sizeof(float)) || audioCodecCtx->sample_fmt == AV_SAMPLE_FMT_FLT)
    {
        if (alIsExtensionPresent("AL_EXT_float32"))
        {
            *pType = AL_FLOAT_SOFT;

            if (*pChannels == 1)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_MONO_FLOAT32");
            }
            else if (*pChannels == 2)
            {
                *pFormat = alGetEnumValue("AL_FORMAT_STEREO_FLOAT32");
            }
            else if (alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                if (*pChannels == 4)
                {
                    *pFormat = alGetEnumValue("AL_FORMAT_QUAD32");
                }
                else if (*pChannels == 6)
                {
                    *pFormat = alGetEnumValue("AL_FORMAT_51CHN32");
                }
                else if (*pChannels == 7)
                {
                    *pFormat = alGetEnumValue("AL_FORMAT_61CHN32");
                }
                else if (*pChannels == 8)
                {
                    *pFormat = alGetEnumValue("AL_FORMAT_71CHN32");
                }
            }
        }
    }
	return 0;
}

}
