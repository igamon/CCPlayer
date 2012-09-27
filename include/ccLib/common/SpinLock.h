#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <pthread.h>

namespace CCPlayer
{

class CCSpinLock
{
public:
    CCSpinLock()
    {
        pthread_spin_init(&m_spinLock, PTHREAD_PROCESS_PRIVATE);
    };
    ~CCSpinLock()
    {
        pthread_spin_destroy(&m_spinLock);
    }

public:
    int Lock()
    {
        return pthread_spin_lock(&m_spinLock);
    }

    int UnLock()
    {
        return pthread_spin_unlock(&m_spinLock);
    }

private:
    pthread_spinlock_t m_spinLock;
};

}

#endif
