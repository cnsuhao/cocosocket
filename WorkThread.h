#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <semaphore.h>
#include "Thread.h"
#include "Task.h"

class WorkThread : public Thread {
protected:
    Task * m_pTask;
    void run();
public:
    WorkThread(int nNo = 0);
    ~WorkThread();
public:
    void setTask(Task * pTask);
};

inline void WorkThread::setTask(Task * pTask) {
    m_pTask = pTask;
}
#endif

