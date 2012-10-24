#include "SystemAlarm.h"

namespace CCPlayer
{

CCMutex CCSystemAlarm::m_instanceMutex;
CCSystemAlarm* CCSystemAlarm::m_pInstance = NULL;

CCSystemAlarm::CCSystemAlarm()
{
    //ctor
}

CCSystemAlarm::~CCSystemAlarm()
{
    //dtor
}

CCSystemAlarm* CCSystemAlarm::GetInstance()
{
    m_instanceMutex.Lock();
    if(m_pInstance == NULL)
    {
        m_pInstance = new CCSystemAlarm();
    }
    m_instanceMutex.UnLock();

    return m_pInstance;
}

}
