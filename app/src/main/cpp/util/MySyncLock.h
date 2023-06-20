//
// Created by dev on 2023/5/25.
//

#include <pthread.h>

#ifndef STUDYOPENGL3_0_MYSYNCLOCK_H
#define STUDYOPENGL3_0_MYSYNCLOCK_H

#include "LogUtils.h"

class MySyncLock {
public:
    MySyncLock() {
        // 初始化互斥锁属性对象
        pthread_mutexattr_init(&m_attr);
        // 设置互斥锁的类型属性
        // PTHREAD_MUTEX_RECURSIVE:
        // 如果线程在不首先解除锁定互斥锁的情况下尝试重新锁定该互斥锁，
        // 则可成功锁定该互斥锁。 与 PTHREAD_MUTEX_NORMAL 类型的互斥锁不同，
        // 对此类型互斥锁进行重新锁定时不会产生死锁情况。
        // 多次锁定互斥锁需要进行相同次数的解除锁定才可以释放该锁，
        // 然后其他线程才能获取该互斥锁。如果线程尝试解除锁定的互斥锁已经由其他线程锁定，
        // 则会返回错误。 如果线程尝试解除锁定的互斥锁未锁定，则会返回错误。
        pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_mutex, &m_attr);

    }

    ~MySyncLock(void) {
        pthread_mutex_destroy(&m_mutex);
        pthread_mutexattr_destroy(&m_attr);
    }

    int Lock() {
        LOGCATI("MySyncLock::Lock()                                             🔐-加锁\n");
        return pthread_mutex_lock(&m_mutex);
    }

    int UnLock() {
        LOGCATI("MySyncLock::UnLock()                                           🔓-解锁\n");
        return pthread_mutex_unlock(&m_mutex);
    }

    int TryLock() {
        return pthread_mutex_trylock(&m_mutex);
    }

private:
    pthread_mutex_t     m_mutex;
    pthread_mutexattr_t m_attr;
};

class ScopedSyncLock {
public:
    // 直接给 m_pLock 赋值了。
    ScopedSyncLock(MySyncLock *pLock) : m_pLock(pLock) {
        if (pLock != NULL) {
            m_pLock->Lock();
        }
    }

    ~ScopedSyncLock() {
        if (m_pLock != NULL)
            m_pLock->UnLock();
    }

private:
    MySyncLock *m_pLock;
};


#endif //STUDYOPENGL3_0_MYSYNCLOCK_H
