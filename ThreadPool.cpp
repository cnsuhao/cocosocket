#include <iostream>
#include "ThreadPool.h"
#include "Thread.h"
using namespace std;

ThreadPool::ThreadPool(int poolsize, int initsize)
: poolSize(poolsize), initsize(initsize), alive(0), pool(NULL)
{
    pool = new WorkThread* [poolsize];
    if (NULL == pool)
    {
        return;
    }
    memset(pool, 0, sizeof (WorkThread*) * poolsize);
    for (int i = 0; i < initsize; ++i)
    {
        pool[i] = new WorkThread();
        ++alive;
    }
}

ThreadPool::~ThreadPool()
{
    if (NULL == pool)
    {
        return;
    }
    for (int i = 0; i < alive; ++i)
    {
        if (NULL == pool[i])
        {
            continue;
        }
        delete pool[i];
    }
    delete [] pool;
}

bool ThreadPool::Offer(Task * pTask)
{
    bool result = false;
    AutoMutex AutoMutex(&lock);
    for (int i = 0; i < alive; ++i)
    {
        if (pool[i]->GetStatus() == Thread::IDLE)
        {
            pool[i]->AddTask(pTask);
            result = true;
            break;
        }
    }
    if (!result && alive < poolSize)
    {
        pool[alive] = new WorkThread();
        pool[alive]->AddTask(pTask);
        ++alive;
        result = true;
    }
    return result;
}

/**
 * 停止线程池，调用之后不能再往线程池里放task，否则
 */
void ThreadPool::Shutdown()
{
    if (NULL == pool)
    {
        return;
    }
    for (int i = 0; i < alive; ++i)
    {
        if (NULL == pool[i])
        {
            continue;
        }
        pool[i]->SetStatus(Thread::QUITED);
        pool[i]->Join();
        delete pool[i];
        pool[i] = NULL;
    }
    delete [] pool;
    alive = 0;
}
