
#ifndef __THREAD_LRL_20130214_HEADER__
#define __THREAD_LRL_20130214_HEADER__

#include <string.h>
#include "WorkThread.h"
#include "Mutex.h"
#include "AutoMutex.h"

class ThreadPool {
    ///属性
public:
    ///nothing.
protected:
    ///nothing.
private:
    ///线程池大小
    int m_nPoolSize;
    ///线程池初始处于挂起状态的线程数
    int m_nInitializeCount;
    ///活动中的线程数
    int m_nAliveCount;
    ///线程数组
    WorkThread** m_pThreads;
    ///保护线程数组的互斥量
    Mutex m_cMutex;
    ///构造函数&& 析构函数
public:
    ThreadPool(int nPoolSize = 128, int nInitializeCount = 0);
    ~ThreadPool();
public:
    bool postTask(Task* pTask);
    int getPoolSize();
    int getInitializeCount();
    int getAliveCount();
    void waitAliveFinish();
};

inline int ThreadPool::getPoolSize() {
    return m_nPoolSize;
}

inline int ThreadPool::getInitializeCount() {
    return m_nInitializeCount;
}

inline int ThreadPool::getAliveCount() {
    return m_nAliveCount;
}

#endif

