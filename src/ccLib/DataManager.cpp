#include "DataManager.h"
#include "MessageCenter.h"

namespace CCPlayer
{

static int ffmpegLockManager( void **mtx, enum AVLockOp op )
{
    switch( op )
    {
        case AV_LOCK_CREATE:
        {
            *mtx =(pthread_mutex_t*) malloc( sizeof(pthread_mutex_t) );

            if( !*mtx ) {
                return 1;
            }

            return !!pthread_mutex_init( (pthread_mutex_t*)*mtx, NULL );
        }

        case AV_LOCK_OBTAIN:
        {
            return !!pthread_mutex_lock( (pthread_mutex_t*) *mtx );
        }

        case AV_LOCK_RELEASE:
        {
            return !!pthread_mutex_unlock( (pthread_mutex_t*)*mtx );
        }

        case AV_LOCK_DESTROY:
        {
            pthread_mutex_destroy( (pthread_mutex_t*)*mtx );
            free( *mtx );
            return 0;
        }
    }

    return 1;
}

CCDataManager::CCDataManager()
{
}

CCDataManager::~CCDataManager()
{
}

void CCDataManager::SendMessage(MessageObjectId messageSender,
                            MessageObjectId messageReceiver,
                            MessageType msg,
                            Any anyParam)
{
    CCMessageCenter::GetInstance()->SendMessage(messageSender,
                                                messageReceiver,
                                                msg,
                                                anyParam);
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

enum DataManagerStatus
{
    DATA_MANAGER_STATUS_ENUM_INIT,
    DATA_MANAGER_STATUS_ENUM_WORKING,
    DATA_MANAGER_STATUS_ENUM_SLEEPING,
    DATA_MANAGER_STATUS_ENUM_DEADING,
    DATA_MANAGER_STATUS_ENUM_MAX
};

void CCDataManager::Run()
{
    //this is register for all ffmpeg action
    av_register_all();

    //this is for mutli-thread ffmpeg working
    if(av_lockmgr_register(ffmpegLockManager))
    {
        // TODO Failure av_lockmgr_register
    }

    std::string mediaUrl;
    AVFormatContext *pAVFormatContext = NULL;
    int asIndex;
    int vsIndex;
    DataManagerStatus status = DATA_MANAGER_STATUS_ENUM_INIT;

    while(m_bRunning)
    {
        SmartPtr<Event> event;
        if(PopFrontMessage(event))
        {
            switch(event.GetPtr()->type)
            {
                case MESSAGE_TYPE_ENUM_OPEN_FILE:
                {
                    mediaUrl = any_cast<std::string>(event.GetPtr()->anyParams);

                    AVCodecContext* pAudioCtx = NULL;
                    AVCodecContext* pVideoCtx = NULL;

                    int ret = OpenFile(mediaUrl,
                                       &pAVFormatContext,
                                       &asIndex,
                                       &vsIndex);
                                        if(ret == 0)
                    if(ret == 0)
                    {
                        GetCodecContext(pAVFormatContext, asIndex, &pAudioCtx);
                        GetCodecContext(pAVFormatContext, vsIndex, &pVideoCtx);

                        status = DATA_MANAGER_STATUS_ENUM_WORKING;
                    }

                    std::vector<Any> openedParams;
                    openedParams.push_back(Any(ret));
                    openedParams.push_back(Any(pAVFormatContext));
                    openedParams.push_back(Any(pAudioCtx));
                    openedParams.push_back(Any(pVideoCtx));

                    SendMessage(MESSAGE_OBJECT_ENUM_DATA_MANAGER,
                                MESSAGE_OBJECT_ENUM_PLAYER,
                                MESSAGE_TYPE_ENUM_OPENED_FILE,
                                openedParams);

                    //just for wait for the decoders working , I will change it
                    Sleep(100);
                }
                break;
                default:
                    std::cout << "Unknow Data Manager Message" << std::endl;
            }
        }

        switch(status)
        {
            case DATA_MANAGER_STATUS_ENUM_INIT:
            {
            }
            break;
            case DATA_MANAGER_STATUS_ENUM_WORKING:
            {
                SmartPtr<CCPacket> packet(new CCPacket());
                if(av_read_frame(pAVFormatContext, packet.GetPtr()->GetPacketPointer()) < 0)
                {
                    m_bRunning = false;
                    continue;
                }

                if(packet.GetPtr()->GetPacketPointer()->stream_index
                            == asIndex)
                {
                    SendMessage(MESSAGE_OBJECT_ENUM_DATA_MANAGER,
                                MESSAGE_OBJECT_ENUM_AUDIO_DECODER,
                                MESSAGE_TYPE_ENUM_GET_AUDIO_PACKET,
                                Any(packet));
                }else if(packet.GetPtr()->GetPacketPointer()->stream_index
                            == vsIndex)
                {
                    SendMessage(MESSAGE_OBJECT_ENUM_DATA_MANAGER,
                                MESSAGE_OBJECT_ENUM_VIDEO_DECODER,
                                MESSAGE_TYPE_ENUM_GET_VIDEO_PACKET,
                                Any(packet));
                }
            }
            break;
            case DATA_MANAGER_STATUS_ENUM_SLEEPING:
            {
            }
            break;
            case DATA_MANAGER_STATUS_ENUM_DEADING:
            {
            }
            break;
        } // end switch

        Sleep(10);
    }
}

int CCDataManager::OpenFile(const std::string& mediaUrl,
                 AVFormatContext** ppFormatCtx,
                 int* pASIndex,
                 int* pVSIndex)
{
    *pASIndex = -1;
    *pVSIndex = -1;
    if(avformat_open_input(ppFormatCtx,mediaUrl.c_str(), NULL, NULL) != 0)
	{
		std::cout << "can not open media file" << std::endl;
		return FAILURE;
	}else
	{
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
		if(*pASIndex != -1
				&& *pVSIndex != -1)
		{
			//We have find the stream
			break;
		}

        if(*pASIndex == -1
                && (*ppFormatCtx)->streams[i]->codec->codec_type
					== AVMEDIA_TYPE_AUDIO)
		{
		    *pASIndex = i;
        }

		if(*pVSIndex == -1
				&& (*ppFormatCtx)->streams[i]->codec->codec_type
					== AVMEDIA_TYPE_VIDEO)
		{
		    *pVSIndex = i;
        }

	}

	return 0;
}

void CCDataManager::GetCodecContext(AVFormatContext* pFormatCtx,
                      int streamIndex,
                      AVCodecContext** ppCodecContext)
{
    *ppCodecContext = pFormatCtx->streams[streamIndex]->codec;
}

}
