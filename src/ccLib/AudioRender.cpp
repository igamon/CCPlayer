#include "AudioRender.h"
#include "MessageCenter.h"
#include "ALWrapper.h"

namespace CCPlayer
{

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
                case MESSAGE_TYPE_ENUM_GET_AUDIO_INFORMATION:
                {
                    std::vector<Any> audioInformartion =
                                        any_cast<std::vector<Any> >(event.GetPtr()->anyParams);

                    CCChannels channels = any_cast<CCChannels>(audioInformartion[0]);
                    CCRates rates = any_cast<CCRates>(audioInformartion[1]);
                    CCType type = any_cast<CCType>(audioInformartion[2]);

                    ALenum audFormat = GetAudioFormat(channels, type);

                    alWrapper.SetAudioCtx(channels, rates, audFormat);
                }
                break;
            case  MESSAGE_TYPE_ENUM_GET_AUDIO_FRAME:
                {
                    SmartPtr<AudioFrame> audioFrame
                                            = any_cast<SmartPtr<AudioFrame> >(event.GetPtr()->anyParams);
                    m_audioFrameQueue.push(audioFrame);
                }
                break;
            } // end switch case
        }

        Sleep(10);
    }
}

ALenum CCAudioRender::GetAudioFormat(CCChannels channels, CCType type)
{
    ALenum audioFormat = 0;
    if (type == CCPlayer::u8)
    {
        if (channels == 1)
        {
            audioFormat = AL_FORMAT_MONO8;
        }
        else if (channels == 2)
        {
            audioFormat = AL_FORMAT_STEREO8;
        }
        else if (alIsExtensionPresent("AL_EXT_MCFORMATS"))
        {
            if (channels == 4)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_QUAD8");
            }
            else if (channels == 6)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_51CHN8");
            }
            else if (channels == 7)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_71CHN8");
            }
            else if (channels == 8)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_81CHN8");
            }
        }
    }
    else if (type == CCPlayer::s16)
    {
        if (channels == 1)
        {
            audioFormat = AL_FORMAT_MONO16;
        }
        else if (channels == 2)
        {
            audioFormat = AL_FORMAT_STEREO16;
        }
        else if (alIsExtensionPresent("AL_EXT_MCFORMATS"))
        {
            if (channels == 4)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_QUAD16");
            }
            else if (channels == 6)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_51CHN16");
            }
            else if (channels == 7)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_61CHN16");
            }
            else if (channels == 8)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_71CHN16");
            }
        }
    }
    else if (type == CCPlayer::f32)
    {
        if (alIsExtensionPresent("AL_EXT_float32"))
        {
            if (channels == 1)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_MONO_FLOAT32");
            }
            else if (channels == 2)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_STEREO_FLOAT32");
            }
            else if (alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                if (channels == 4)
                {
                    audioFormat = alGetEnumValue("AL_FORMAT_QUAD32");
                }
                else if (channels == 6)
                {
                    audioFormat = alGetEnumValue("AL_FORMAT_51CHN32");
                }
                else if (channels == 7)
                {
                    audioFormat = alGetEnumValue("AL_FORMAT_61CHN32");
                }
                else if (channels == 8)
                {
                    audioFormat = alGetEnumValue("AL_FORMAT_71CHN32");
                }
            }
        }
    }
    else if (type == CCPlayer::d64)
    {
        if (alIsExtensionPresent("AL_EXT_double"))
        {
            if (channels == 1)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_MONO_DOUBLE_EXT");
            }
            else if (channels == 2)
            {
                audioFormat = alGetEnumValue("AL_FORMAT_STEREO_DOUBLE_EXT");
            }
        }
    }
    if (type == 0)
    {
        std::cout << "Unfinded format for the audio " << std::endl;
    }

    return audioFormat;
}


}
