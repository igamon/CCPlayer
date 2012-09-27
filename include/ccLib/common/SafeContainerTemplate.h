#ifndef SAFECONTAINERTEMPLATE_H
#define SAFECONTAINERTEMPLATE_H

#include <pthread.h>

namespace CCPlayer
{

template <typename Container>
class ContainerLock
{
public:
    ContainerLock(Container& container)
    :m_container(container)
    {
        pthread_spinlock_init(&m_spinLock, PTHREAD_PROCESS_PRIVATE);
        pthread_spinlock_lock(&m_spinLock);
    }
    ~ContainerLock()
    {
        pthread_spinlock_unlock(&m_spinLock);
        pthread_spinlock_destory(&m_spinLock);
    }

private:
    pthread_spinlock_t m_spinLock;
private:
    Container& m_container;
};

}
#endif
