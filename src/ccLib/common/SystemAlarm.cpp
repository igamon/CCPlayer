#include "SystemAlarm.h"
#include "FrequencyWorker.h"

namespace CCPlayer
{

CCMutex CCSystemAlarm::m_instanceMutex;
CCSystemAlarm* CCSystemAlarm::m_pInstance = NULL;

CCSystemAlarm::CCSystemAlarm()
{
    //ctor
    //every 15 ms tell the worker
    int timeId = 0;
    m_frequencyTimer.SetTimer(&timeId, 15, this);
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


void CCSystemAlarm::RegisterSystemAlarm(CCFrequencyWorker* pFrequencyWorker)
{
    m_frequencyWorkerMutex.Lock();
    m_frequencyWorkerList.push_back(pFrequencyWorker);
    m_frequencyWorkerMutex.UnLock();
}

void CCSystemAlarm::UnRegisterSystemAlarm(CCFrequencyWorker* pFrequencyWorker)
{
    m_frequencyWorkerMutex.Lock();

    std::vector<CCFrequencyWorker*>::iterator pos = m_frequencyWorkerList.begin();
    while(pos != m_frequencyWorkerList.end())
    {
        if(*pos == pFrequencyWorker)
        {
            break;
        }
        pos ++;
    }

    m_frequencyWorkerList.erase(pos);

    m_frequencyWorkerMutex.UnLock();
}

void CCSystemAlarm::TimeElapsed()
{
    m_frequencyWorkerMutex.Lock();

    std::vector<CCFrequencyWorker*>::iterator pos = m_frequencyWorkerList.begin();
    while(pos != m_frequencyWorkerList.end())
    {
        //Wakeup the thread
        (*pos)->Post();
        pos ++;
    }

    m_frequencyWorkerMutex.UnLock();
}

}
