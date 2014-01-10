/**
 *	Author: ACb0y
 *	FileName: WorkThread.h
 *	CreateTime: 2013年2月14日12:20:43
 *	Version: 1.0
 */

#ifndef __WORK_THREAD_LRL_20130214_HEADER__
#define __WORK_THREAD_LRL_20130214_HEADER__

#include <semaphore.h>
#include "Thread.h"
#include "Task.h"

/*
    工作线程
 */
class WorkThread : public CThread {
    ///属性
public:
    ///nothing.
protected:
    ///线程编号
    int m_nNo;
    int m_nStatus;
    sem_t * m_pSem;
    bool m_bNeedQuit;
    bool m_bAutoFinish;
    Task * m_pTask;
private:
    ///nothing.

    ///构造函数&&析构函数
public:
    WorkThread(int nNo = 0);
    ~WorkThread();
protected:
    ///nothing.
private:
    ///nothing.

    ///服务
public:
    int getNo();
    int getStatus();
    ///创建线程并将线程挂起
    ///调用成功返回IDLE
    int initialize();
    void setTask(Task * pTask);
    ///设置线程执行一次后自动退出
    void setAutoFinish();
    ///释放信号量让线程恢复运行，启动成功返回RUNNING
    ///当Task执行结束后，线程自动变成IDLE状态
    ///如果之前有调用setAutoFinish，则线程自动结束，状态变成了QUITED
    int run();
    ///通知并等待线程退出
    void finish();
protected:
    static void * doRun(void * pContext);
private:
    ///nothing.
};

inline void WorkThread::setTask(Task * pTask) {
    m_pTask = pTask;
}

inline int WorkThread::getNo() {
    return m_nNo;
}

inline int WorkThread::getStatus() {
    return m_nStatus;
}

inline void WorkThread::setAutoFinish() {
    m_bAutoFinish = true;
}
#endif

