#include "Thread.h"

Thread::Thread() : threadId(NULL), status(UNINITIALIZED), sem(NULL)
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
    nStatus = pthread_create(&threadId, &sThread_attr, pFuncStartRoutine, pArg);
    pthread_attr_destroy(&sThread_attr);
    return nStatus;
}

/**
 * 初始化
 * @return 
 */
int Thread::initialize()
{
    sem = new sem_t;
    if (sem_init(sem, 0, 0) < 0)
    {
        return ERR_AT_CREATE_SEM;
    }
    if (create(&doRun, (void *) this) < 0)
    {
        return ERR_AT_CREATE_THREAD;
    }
    status = IDLE;
    return status;
}

void * Thread::doRun(void* context)
{
    Thread * thread = (Thread *) context;
    sem_wait(thread->sem);
    if (RUNNING == thread->status)
    {
        thread->run();
    }
    thread->status = QUITED;
    return (void *) 0;
}

/**
 *  启动线程
 * @return 
 */
int Thread::start()
{
    if (status != IDLE)
    {
        return ERR_NOT_IDLE;
    }
    status = RUNNING;
    sem_post(sem);
    return status;
}

/**
 * 结束
 */
void Thread::finish()
{
    if (status != UNINITIALIZED && status != QUITED)
    {
        sem_post(sem);
        sem_destroy(sem);
        delete sem;
        sem = NULL;
    }
}


