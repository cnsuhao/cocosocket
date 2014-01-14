#include "Thread.h"

Thread::Thread(int nNo) : m_nThreadId(NULL), m_nStatus(UNINITIALIZED), m_bNeedQuit(false), m_bAutoFinish(false), m_nNo(nNo), m_pSem(NULL)
{
    this->initialize();
}

Thread::~Thread()
{
    finish();
}

/**
 * 创建线程
 * @param pFuncStartRoutine
 * @param pArg
 * @param bDetached
 * @param bSetScope
 * @return 
 */
int Thread::create(pFuncThreadStart pFuncStartRoutine, void * pArg, bool bDetached, bool bSetScope)
{
    pthread_attr_t sThread_attr;
    int nStatus;
    nStatus = pthread_attr_init(&sThread_attr);
    if (nStatus != 0)
    {
        return -1;
    }
    if (bDetached)
    {
        nStatus = pthread_attr_setdetachstate(&sThread_attr, PTHREAD_CREATE_DETACHED);
        if (nStatus != 0)
        {
            pthread_attr_destroy(&sThread_attr);
            return -1;
        }
    }
    if (bSetScope)
    {
        nStatus = pthread_attr_setscope(&sThread_attr, PTHREAD_SCOPE_SYSTEM);
        if (nStatus != 0)
        {
            pthread_attr_destroy(&sThread_attr);
            return -1;
        }
    }
    nStatus = pthread_create(&m_nThreadId, &sThread_attr, pFuncStartRoutine, pArg);
    pthread_attr_destroy(&sThread_attr);
    return nStatus;
}

/**
 * 初始化
 * @return 
 */
int Thread::initialize()
{
    m_pSem = new sem_t;
    if (m_nStatus != UNINITIALIZED && m_nStatus != QUITED)
    {
        return ERR_ALREADERY_INITIALIZED;
    }
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

void * Thread::doRun(void * pArg)
{
    Thread * pWorkThread = (Thread *) pArg;
    while (!pWorkThread->m_bNeedQuit)
    {
        sem_wait(pWorkThread->m_pSem);
        if (RUNNING == pWorkThread->m_nStatus)
        {
            pWorkThread->run();
            pWorkThread->m_nStatus = IDLE;
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

/**
 *  启动线程
 * @return 
 */
int Thread::start()
{
    if (m_nStatus != IDLE)
    {
        return ERR_NOT_IDLE;
    }
    m_nStatus = RUNNING;
    sem_post(m_pSem);
    return m_nStatus;
}

/**
 * 结束
 */
void Thread::finish()
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

