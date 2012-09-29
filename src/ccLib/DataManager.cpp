#include "DataManager.h"
#include "MessageCenter.h"

namespace CCPlayer
{

CCDataManager::CCDataManager()
{
    //this is register for all ffmpeg action
    av_register_all();
}

CCDataManager::~CCDataManager()
{
}

void CCDataManager::SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam)
{
    CCMessageCenter::GetInstance()->SendMessage(messageSender, messageReceiver, msg, anyParam);
}

void CCDataManager::ReceiverMessage(const SmartPtr<Event>& rSmtEvent)
{
    m_spinLockMessageQueue.Lock();
    m_messageQueue.push(rSmtEvent);
    m_spinLockMessageQueue.UnLock();
}

bool CCDataManager::PopFrontMessage(SmartPtr<Event>& rSmtEvent)
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

void CCDataManager::Run()
{
    AVFormatContext *pAVFormatContext = NULL;
	int	videoStreamIndex = -1;
	int	audioStreamIndex = -1;

    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            switch(event.GetPtr()->type)
            {
                case MESSAGE_TYPE_ENUM_OPEN_FILE:
                {
                    std::string mediaUrl = any_cast<std::string>(event.GetPtr()->anyParams);
                    int ret = OpenFile(mediaUrl, &pAVFormatContext, &videoStreamIndex, &audioStreamIndex);

                    std::vector<Any> openedParams;
                    openedParams.push_back(Any(ret));
                    openedParams.push_back(Any(pAVFormatContext));
                    openedParams.push_back(Any(videoStreamIndex));
                    openedParams.push_back(Any(audioStreamIndex));

                    SendMessage(MESSAGE_OBJECT_ENUM_DATA_MANAGER, MESSAGE_OBJECT_ENUM_PLAYER, MESSAGE_TYPE_ENUM_OPENED_FILE, openedParams);
                    //std::vector<Any> openParam = any_cast<std::vector<Any> >(event.GetPtr()->anyParams);
                    //std::cout << any_cast<std::string>(openParam[0]) << std::endl;
                    //std::cout << std::hex << any_cast<AVFormatContext**>(openParam[1]) << std::endl;
                }
                break;
                default:
                    std::cout << "Unknow Data Manager Message" << std::endl;
            }

            //std::cout << "CCDataManager" << any_cast<std::string>(event.GetPtr()->anyParams) << std::endl;
        }

        Sleep(10);
    }
}

int CCDataManager::OpenFile(const std::string& mediaUrl, AVFormatContext** ppFormatCtx, int* pVSIndex, int* pASIndex)
{
    if(avformat_open_input(ppFormatCtx,mediaUrl.c_str(), NULL, NULL) != 0)
	{
		std::cout << "can not open media file" << std::endl;
		return FAILURE;
	}else{
		std::cout << "open media file" << std::endl;
	}

	if(avformat_find_stream_info(*ppFormatCtx, NULL) < 0)
	{
		std::cout << "can not retrieve file info" << std::endl;
		return FAILURE;
	}else
	{
		std::cout << "retrieve file info" << std::endl;
	}

	av_dump_format(*ppFormatCtx,
			0, mediaUrl.c_str(), 0);

	for(unsigned i=0; i<(*ppFormatCtx)->nb_streams; i++)
	{
		if(*pVSIndex != -1
				&& *pASIndex != -1)
		{
			//We have find the stream
			break;
		}

		if(*pVSIndex == -1
				&& (*ppFormatCtx)->streams[i]->codec->codec_type
					== AVMEDIA_TYPE_VIDEO)
		{
			*pVSIndex = i;
        }

		if(*pASIndex == -1
				&& (*ppFormatCtx)->streams[i]->codec->codec_type
					== AVMEDIA_TYPE_AUDIO)
		{
			*pASIndex = i;
        }
	}
}


}
