
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <string.h>
#include "WorkThread.h"
#include "Mutex.h"
#include "AutoMutex.h"

class ThreadPool {
private:
    ///线程池大小
    int poolSize;
    ///线程池初始处于挂起状态的线程数
    int initsize;
    ///活动中的线程数
    int alive;
    ///线程数组
    WorkThread** pool;
    ///保护线程数组的互斥量
    Mutex lock;
    ///构造函数&& 析构函数
public:
    ThreadPool(int ps = 128, int ic = 0);
    ~ThreadPool();
public:
    bool offer(Task* pTask);
    int getPoolSize();
    int getInitializeCount();
    int getAliveCount();
    void shutdown();
};

inline int ThreadPool::getPoolSize() {
    return poolSize;
}

inline int ThreadPool::getInitializeCount() {
    return initsize;
}

inline int ThreadPool::getAliveCount() {
    return alive;
}
#endif

