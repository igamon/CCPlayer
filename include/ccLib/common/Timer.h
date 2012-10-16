#ifndef TIMER_H
#define TIMER_H

#include "Common.h"

namespace CCPlayer
{

class ITimerInterface
{
public:
    virtual void TimeElapsed();
};

class CCTimer
{
public:
    CCTimer();
    virtual ~CCTimer();

public:
    bool SetTimer(int* timerId,
                  unsigned int timerInterval,
                  ITimerInterface* pCallTarget);


public:
    static void PASCAL TimerCallBack(unsigned int timerId,
                       unsigned int msg,
                       unsigned long userData,
                       unsigned long define1,
                       unsigned long define2);

private:
    unsigned int m_accuracy;
};

}

#endif // TIMER_H
