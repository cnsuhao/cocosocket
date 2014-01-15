#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>

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
    pthread_t threadId;
    int create(pFuncThreadStart pFuncStartRoutine, void * context, bool bDetached = false, bool bSetScope = false);
    int initialize();
    void finish();
    static void * doRun(void * pContext);
protected:
    int status;
    sem_t* sem;
    virtual void run() = 0;
public:
    Thread();
    virtual ~Thread();
    int detach();
    int join(void ** pRetValue = NULL);
    void exit(void * pRetValue = NULL);
    void yield();
    bool isCurrent();
    pthread_t getThreadId();
    int getStatus();
    int start();
};

inline pthread_t Thread::getThreadId() {
    return threadId;
}

inline int Thread::detach() {
    return pthread_detach(threadId);
}

inline int Thread::join(void ** pRetValue) {
    return pthread_join(threadId, pRetValue);
}

inline void Thread::exit(void * pRetValue) {
    if (isCurrent()) {
        pthread_exit(pRetValue);
    }
}

inline bool Thread::isCurrent() {
    if (pthread_equal(threadId, pthread_self()) != 0) {
        return true;
    } else {
        return false;
    }
}

inline void Thread::yield() {
    sched_yield();
}

inline int Thread::getStatus() {
    return status;
}
#endif


