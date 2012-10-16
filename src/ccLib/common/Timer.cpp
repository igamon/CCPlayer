#include "Timer.h"

namespace CCPlayer
{

#define TIMER_ACCURACY 1

CCTimer::CCTimer()
{
    TIMECAPS tc;

    //利用函数timeGetDeVCaps取出系统分辨率的取值范围，如果无错则继续
    if(timeGetDevCaps(&tc,sizeof(TIMECAPS))==TIMERR_NOERROR)
    {
        //分辨率的值不能超出系统的取值范围
        m_accuracy = std::max(tc.wPeriodMin, (unsigned int)TIMER_ACCURACY);
        m_accuracy = std::min(m_accuracy, tc.wPeriodMax);

        //调用timeBeginPeriod函数设置定时器的分辨率
        timeBeginPeriod(m_accuracy);
    }
}

CCTimer::~CCTimer()
{
    timeEndPeriod(m_accuracy);
}

bool CCTimer::SetTimer(int* timerId,
                       unsigned int timerInterval,
                       ITimerInterface* pCallTarget)
{
    if((*timerId = timeSetEvent(timerInterval,
                                m_accuracy,
                                (LPTIMECALLBACK)&TimerCallBack,
                                (unsigned long)pCallTarget,
                                TIME_PERIODIC)) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CCTimer::TimerCallBack(unsigned int timerId,
                          unsigned int msg,
                          unsigned long userData,
                          unsigned long define1,
                          unsigned long define2)
{
    ITimerInterface* pTimerObject = reinterpret_cast<ITimerInterface*>(userData);
    pTimerObject->TimeElapsed();
}

}
