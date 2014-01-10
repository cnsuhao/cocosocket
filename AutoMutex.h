

#ifndef __AUTO_MUTEX_LRL_20130214_HEADER__
#define __AUTO_MUTEX_LRL_20130214_HEADER__

#include "Mutex.h"

/*
	
 */
class AutoMutex {
    ///属性
public:
    ///nothing.
protected:
    ///nothing.
private:
    Mutex * m_pMutex;
    ///构造函数&& 析构函数
public:
    AutoMutex(Mutex * pMutex) {
        m_pMutex = pMutex;
        m_pMutex->lock();
    }
    ~AutoMutex() {
        m_pMutex->unlock();
    }
protected:
    ///nothing.
private:
    ///nothing.

    ///服务
public:
    ///nothing.
protected:
    ///nothing.
private:
    ///nothing.
};
#endif

