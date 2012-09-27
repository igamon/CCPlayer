#include "ModuleManager.h"
#include "DataManager.h"
#include "MessageCenter.h"

namespace CCPlayer
{

CCModuleManager::CCModuleManager()
{
}

CCModuleManager::~CCModuleManager()
{
}

void CCModuleManager::AddModule(MessageObjectId messageObjectId)
{
    switch(messageObjectId)
    {
        case MESSAGE_OBJECT_ENUM_VIDEO_RENDER:
            break;
        case MESSAGE_OBJECT_ENUM_AUDIO_RENDER:
            break;
        case MESSAGE_OBJECT_ENUM_VIDEO_DECODER:
            break;
        case MESSAGE_OBJECT_ENUM_AUDIO_DECODER:
            break;
        case MESSAGE_OBJECT_ENUM_DATA_MANAGER:
            {
                CCDataManager* pDataManager = new CCDataManager();
                pDataManager->Launch();
                CCMessageCenter::GetInstance()->RegisterMessageReceiver(messageObjectId, pDataManager);
            }
            break;
        default:
            printf("Unknow message object id\n");
            break;
    }
}

void CCModuleManager::DeleteModule(MessageObjectId messageObjectId)
{
    switch(messageObjectId)
    {
        case MESSAGE_OBJECT_ENUM_VIDEO_RENDER:
            break;
        case MESSAGE_OBJECT_ENUM_AUDIO_RENDER:
            break;
        case MESSAGE_OBJECT_ENUM_VIDEO_DECODER:
            break;
        case MESSAGE_OBJECT_ENUM_AUDIO_DECODER:
            break;
        case MESSAGE_OBJECT_ENUM_DATA_MANAGER:
            {
                IMessageReceiver* pMessageReceiver = CCMessageCenter::GetInstance()->GetMessageReceiver(messageObjectId);

                CCMessageCenter::GetInstance()->UnRegisterMessageReceiver(messageObjectId);

                if(pMessageReceiver != NULL)
                {
                    delete pMessageReceiver;
                    pMessageReceiver = NULL;
                }

            }
            break;
        default:
            printf("Unknow message object id\n");
            break;
    }
}


}
