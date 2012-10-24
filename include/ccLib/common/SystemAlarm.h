#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include "Timer.h"
#include "Mutex.h"
#include "RWLock.h"

namespace CCPlayer
{

class CCSystemAlarm
{
public:
    CCSystemAlarm();
    virtual ~CCSystemAlarm();

public:


public:
    static CCSystemAlarm* GetInstance();

private:
    CCRWLock m_timerRWLock;

private:
    static CCMutex m_instanceMutex;
    static CCSystemAlarm* m_pInstance;
};

}

#endif // TIMEMANAGER_H
