#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>

typedef void * (* func)(void *);

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
    int Create(func pFuncStartRoutine, void * context, bool bDetached = false, bool bSetScope = false);
    void End();
    int Init();
    static void * DoRun(void * pContext);
protected:
    int status;
    sem_t* sem;
public:
    Thread();
    virtual ~Thread();
    virtual void Run() = 0;
    int Detach();
    int Join(void ** pRetValue = NULL);
    void Exit(void * pRetValue = NULL);
    void Yield();
    bool IsCurrent();
    pthread_t GetThreadId();
    int GetStatus();
    int Start();
};

inline pthread_t Thread::GetThreadId() {
    return threadId;
}

inline int Thread::Detach() {
    return pthread_detach(threadId);
}

inline int Thread::Join(void ** pRetValue) {
    return pthread_join(threadId, pRetValue);
}

inline void Thread::Exit(void * pRetValue) {
    if (IsCurrent()) {
        pthread_exit(pRetValue);
    }
}

inline bool Thread::IsCurrent() {
    if (pthread_equal(threadId, pthread_self()) != 0) {
        return true;
    } else {
        return false;
    }
}

inline void Thread::Yield() {
    sched_yield();
}

inline int Thread::GetStatus() {
    return status;
}
#endif


