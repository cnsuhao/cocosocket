#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <semaphore.h>
#include "Thread.h"
#include "Task.h"

class WorkThread : public Thread {
protected:
    void run();
public:
    WorkThread();
    ~WorkThread();
    void setTask(Task * pTask);
private:
    Task* task;
};

inline void WorkThread::setTask(Task * t) {
    task = t;
    sem_post(this->sem);
}
#endif

