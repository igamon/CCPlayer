#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include "Timer.h"
#include "Mutex.h"
#include "RWLock.h"

namespace CCPlayer
{

class CCFrequencyWorker;

class CCSystemAlarm : public ITimerInterface
{
public:
    CCSystemAlarm();
    virtual ~CCSystemAlarm();

public:
    void RegisterSystemAlarm(CCFrequencyWorker* pFrequencyWorker);
    void UnRegisterSystemAlarm(CCFrequencyWorker* pFrequencyWorker);

public:
    static CCSystemAlarm* GetInstance();

private:
    CCTimer m_frequencyTimer;
    CCRWLock m_timerRWLock;

public:
    virtual void TimeElapsed();

private:
    std::vector<CCFrequencyWorker*> m_frequencyWorkerList;
    CCMutex m_frequencyWorkerMutex;

private:
    static CCMutex m_instanceMutex;
    static CCSystemAlarm* m_pInstance;
};

}

#endif // TIMEMANAGER_H
