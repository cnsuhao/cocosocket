#include <iostream>
#include "ThreadPool.h"
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

bool ThreadPool::offer(Task * pTask)
{
    bool result = false;
    AutoMutex AutoMutex(&lock);
    for (int i = 0; i < alive; ++i)
    {
        if (pool[i]->getStatus() == Thread::IDLE)
        {
            pool[i]->setTask(pTask);
            result = true;
            break;
        }
    }
    if (!result && alive < poolSize)
    {
        pool[alive] = new WorkThread();
        pool[alive]->setTask(pTask);
        ++alive;
        result = true;
    }
    return result;
}

void ThreadPool::shutdown()
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
        pool[i] = NULL;
    }
    delete [] pool;
    alive = 0;
}
