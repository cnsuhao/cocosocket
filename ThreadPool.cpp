/**
 *	Author: ACb0y
 *	FileName: ThreadPool.cpp
 *	CreateTime: 2013年2月14日16:57:49
 *	Version: 1.0
 */

#include <iostream>
#include "ThreadPool.h"
using namespace std;

ThreadPool::ThreadPool(int nPoolSize, int nInitializeCount)
: m_nPoolSize(nPoolSize), m_nInitializeCount(nInitializeCount), m_nAliveCount(0), m_pThreads(NULL)
{
    m_pThreads = new WorkThread * [nPoolSize];
    if (NULL == m_pThreads)
    {
        return;
    }

    memset(m_pThreads, 0, sizeof (WorkThread *) * nPoolSize);
    for (int i = 0; i < m_nInitializeCount; ++i)
    {
        m_pThreads[i] = new WorkThread(i + 1);
        if (NULL == m_pThreads)
        {
            break;
        }
        if (m_pThreads[i]->initialize() != CThread::IDLE)
        {
            break;
        }
        ++m_nAliveCount;
    }
}

ThreadPool::~ThreadPool()
{
    if (NULL == m_pThreads)
    {
        return;
    }

    for (int i = 0; i < m_nAliveCount; ++i)
    {
        if (NULL == m_pThreads[i])
        {
            continue;
        }
        m_pThreads[i]->finish();
        delete m_pThreads[i];
        m_pThreads[i] = NULL;
    }
    delete [] m_pThreads;
    m_pThreads = NULL;
}

bool ThreadPool::postTask(Task * pTask)
{
    bool bPostSuccess = false;
    AutoMutex cAutoMutex(&m_cMutex);
    for (int i = 0; i < m_nAliveCount; ++i)
    {
        if (m_pThreads[i]->getStatus() == CThread::IDLE)
        {
            m_pThreads[i]->setTask(pTask);
            m_pThreads[i]->run();
            bPostSuccess = true;
            break;
        }
    }

    if (!bPostSuccess && m_nAliveCount < m_nPoolSize)
    {
        m_pThreads[m_nAliveCount] = new WorkThread(m_nAliveCount + 1);
        if (m_pThreads[m_nAliveCount] != NULL)
        {
            if (m_pThreads[m_nAliveCount]->initialize() == CThread::IDLE)
            {
                m_pThreads[m_nAliveCount]->setTask(pTask);
                m_pThreads[m_nAliveCount]->run();
                ++m_nAliveCount;
                bPostSuccess = true;
            }
        }
    }

    return bPostSuccess;
}

void ThreadPool::waitAliveFinish()
{
    if (NULL == m_pThreads)
    {
        return;
    }

    ///依次调用所有的活动线程，等待运行结束
    for (int i = 0; i < m_nAliveCount; ++i)
    {
        if (NULL == m_pThreads[i])
        {
            continue;
        }
        m_pThreads[i]->finish();
        delete m_pThreads[i];
        m_pThreads[i] = NULL;
    }

    delete [] m_pThreads;
    m_nAliveCount = 0;
}

