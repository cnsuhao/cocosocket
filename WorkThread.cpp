/**
 *	Author: ACb0y
 *	FileName: WorkThread.cpp
 *	CreateTime: 2013年2月14日13:01:35
 *	Version: 1.0
 */

#include "WorkThread.h"
#include "errno.h"

WorkThread::WorkThread(int nNo)
: m_pTask(NULL), m_nStatus(UNINITIALIZED), m_bNeedQuit(false), m_bAutoFinish(false), m_nNo(nNo), m_pSem(NULL)
{
    ///nothing.
}

WorkThread::~WorkThread()
{
    finish();
}

int WorkThread::initialize()
{
    m_pSem = new sem_t;
    if (m_nStatus != UNINITIALIZED && m_nStatus != QUITED)
    {
        return ERR_ALREADERY_INITIALIZED;
    }

    ///创建一个线程间共享初值为0的匿名信号量
    if (sem_init(m_pSem, 0, 0) < 0)
    {
        return ERR_AT_CREATE_SEM;
    }

    if (create(&doRun, (void *) this) < 0)
    {
        return ERR_AT_CREATE_THREAD;
    }

    if (m_bNeedQuit)
    {
        m_bNeedQuit = false;
    }

    if (m_bAutoFinish)
    {
        m_bAutoFinish = false;
    }

    m_nStatus = IDLE;
    return m_nStatus;
}

int WorkThread::run()
{
    if (m_nStatus != IDLE)
    {
        return ERR_NOT_IDLE;
    }

    if (NULL == m_pTask)
    {
        return ERR_NO_TASK;
    }

    m_nStatus = RUNNING;
    sem_post(m_pSem);
    return m_nStatus;
}

void WorkThread::finish()
{
    if (m_nStatus != UNINITIALIZED && m_nStatus != QUITED)
    {
        m_bNeedQuit = true;
        sem_post(m_pSem);
        reset();
        sem_destroy(m_pSem);
        delete m_pSem;
        m_pSem = NULL;
    }
}

void * WorkThread::doRun(void * pArg)
{
    WorkThread * pWorkThread = (WorkThread *) pArg;
    Task * pTask = pWorkThread->m_pTask;

    while (!pWorkThread->m_bNeedQuit)
    {
        sem_wait(pWorkThread->m_pSem);
        if (RUNNING == pWorkThread->m_nStatus)
        {
            if (NULL == pTask)
            {
                pWorkThread->m_nStatus = ERR_NO_TASK;
            } else
            {
                ///执行任务
                pTask->run();
                pWorkThread->m_nStatus = IDLE;
            }
        }

        if (pWorkThread->m_bAutoFinish)
        {
            pWorkThread->detach();
            break;
        }
    }

    pWorkThread->m_nStatus = QUITED;
    return (void *) 0;
}