#include "Timer.h"

namespace CCPlayer
{

#define TIMER_ACCURACY 1

CCTimer::CCTimer()
{
    TIMECAPS tc;

    //���ú���timeGetDeVCapsȡ��ϵͳ�ֱ��ʵ�ȡֵ��Χ������޴������
    if(timeGetDevCaps(&tc,sizeof(TIMECAPS))==TIMERR_NOERROR)
    {
        //�ֱ��ʵ�ֵ���ܳ���ϵͳ��ȡֵ��Χ
        m_accuracy = std::max(tc.wPeriodMin, (unsigned int)TIMER_ACCURACY);
        m_accuracy = std::min(m_accuracy, tc.wPeriodMax);

        //����timeBeginPeriod�������ö�ʱ���ķֱ���
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
