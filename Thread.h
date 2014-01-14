#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include <sched.h>

typedef void * (* pFuncThreadStart)(void *);

class Thread {
public:

    enum ThreadState {
        ERR_ALREADERY_INITIALIZED = -6,
        ERR_AT_CREATE_THREAD = -5,
        ERR_AT_CREATE_SEM = -4,
        ERR_NO_TASK = -3,
        ERR_NOT_IDLE = -2,
        UNINITIALIZED = -1,
        IDLE = 0,
        RUNNING = 1,
        QUITED = 9
    };
private:
    pthread_t m_nThreadId;
    int create(pFuncThreadStart pFuncStartRoutine, void * pArg, bool bDetached = false, bool bSetScope = false);
    ///创建线程并将线程挂起
    ///调用成功返回IDLE
    int initialize();
    ///通知并等待线程退出
    void finish();
    static void * doRun(void * pContext);
protected:
    int m_nNo;
    int m_nStatus;
    sem_t* m_pSem;
    bool m_bNeedQuit;
    bool m_bAutoFinish;
    virtual void run();
public:
    Thread();
    virtual ~Thread();
    int detach();
    int join(void ** pRetValue = NULL);
    void exit(void * pRetValue = NULL);
    void yield();
    void reset();
    bool isCurrent();
    pthread_t getThreadId();
    int getNo();
    int getStatus();
    ///设置线程执行一次后自动退出
    void setAutoFinish();
    int start();
};

inline pthread_t Thread::getThreadId() {
    return m_nThreadId;
}

inline int Thread::detach() {
    return pthread_detach(m_nThreadId);
}

inline int Thread::join(void ** pRetValue) {
    return pthread_join(m_nThreadId, pRetValue);
}

inline void Thread::exit(void * pRetValue) {
    if (isCurrent()) {
        pthread_exit(pRetValue);
    }
}

inline bool Thread::isCurrent() {
    if (pthread_equal(m_nThreadId, pthread_self()) != 0) {
        return true;
    } else {
        return false;
    }
}

inline void Thread::yield() {
    sched_yield();
}

inline void Thread::reset() {
    join();
    m_nThreadId = NULL;
}

inline int Thread::getNo() {
    return m_nNo;
}

inline int Thread::getStatus() {
    return m_nStatus;
}

inline void Thread::setAutoFinish() {
    m_bAutoFinish = true;
}
#endif


