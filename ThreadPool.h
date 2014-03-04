#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <string.h>
#include "WorkThread.h"
#include "Mutext.h"

class ThreadPool {
private:
    int poolSize;
    int initsize;
    int alive;
    WorkThread** pool;
    Mutext* lock;
public:
    ThreadPool(int ps = 1, int ic = 1);
    ~ThreadPool();
public:
    bool Offer(Thread* task);
    int GetPoolSize();
    int GetInitializeCount();
    int GetAliveCount();
    void Shutdown();
};

inline int ThreadPool::GetPoolSize() {
    return poolSize;
}

inline int ThreadPool::GetInitializeCount() {
    return initsize;
}

inline int ThreadPool::GetAliveCount() {
    return alive;
}
#endif

