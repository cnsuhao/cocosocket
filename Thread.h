/**
 *	Author: ACb0y
 *	FileName: Thread.h
 *	CreateTime: 2013年2月13日12:24:02
 *	Version: 1.0
 */

#ifndef __THREAD_LRL_20130213_HEADER__
#define __THREAD_LRL_20130213_HEADER__

#include <pthread.h>
#include <sched.h>

///定义线程启动函数指针
typedef void * (* pFuncThreadStart)(void *);

/*
    对POSIX线程操作的C++封装
 */
class CThread {
    ///属性
public:

    enum EThreadState {
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
protected:
    ///nothing.
private:
    pthread_t m_nThreadId;

    ///构造函数&&虚构函数
public:
    CThread();
    virtual ~CThread();
protected:
    ///nothing.
private:
    ///nothing.

    ///服务
public:
    int create(pFuncThreadStart pFuncStartRoutine, void * pArg, bool bDetached = false, bool bSetScope = false);
    int detach();
    int join(void ** pRetValue = NULL);
    void exit(void * pRetValue = NULL);
    void yield();
    void reset();
    bool isCurrent();
    pthread_t getThreadId();
protected:
    ///nothing.
private:
    ///nothing.
};

inline pthread_t CThread::getThreadId() {
    return m_nThreadId;
}

inline int CThread::detach() {
    return pthread_detach(m_nThreadId);
}

inline int CThread::join(void ** pRetValue) {
    return pthread_join(m_nThreadId, pRetValue);
}

inline void CThread::exit(void * pRetValue) {
    if (isCurrent()) {
        pthread_exit(pRetValue);
    }
}

inline bool CThread::isCurrent() {
    if (pthread_equal(m_nThreadId, pthread_self()) != 0) {
        return true; ///表明是同一线程
    } else {
        return false;
    }
}

inline void CThread::yield() {
    sched_yield();
}

inline void CThread::reset() {
    join();
    m_nThreadId = NULL;
}


#endif


