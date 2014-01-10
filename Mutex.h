/**
 *	Author: ACb0y
 *	FileName: Mutex.h
 *	CreateTime: 2013年2月4日9:33:57
 *	Version: 1.0
 */

#ifndef __MUTEX_LRL_20130204_HEADER__
#define __MUTEX_LRL_20130204_HEADER__

#include <pthread.h>

class Mutex {
    ///属性
public:
    ///nothing.
protected:
    ///nothing.
private:
    pthread_mutex_t m_sMutex;

    ///构造函数&&析构函数
public:
    Mutex(int nShared = PTHREAD_PROCESS_PRIVATE, int nType = PTHREAD_MUTEX_NORMAL);
    ~Mutex();
protected:
    ///nothing.
private:
    ///禁止用户调用拷贝构造函数和赋值操作，调用时会编译报错

    Mutex(const Mutex & cMutex) {
        ///nothing.
    }

    Mutex & operator =(const Mutex & cMutex) {
        return *this;
    }

public:
    int lock();
    int unlock();
    int trylock();
protected:
    ///nothing.
private:
    ///nothing.
};

inline int Mutex::lock() {
    return pthread_mutex_lock(&m_sMutex);
}

inline int Mutex::unlock() {
    return pthread_mutex_unlock(&m_sMutex);
}

inline int Mutex::trylock() {
    return pthread_mutex_trylock(&m_sMutex);
}

#endif
