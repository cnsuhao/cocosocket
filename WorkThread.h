#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <semaphore.h>
#include "Thread.h"
#include "Task.h"

class WorkThread : public Thread {
protected:
    void Run();
public:
    WorkThread();
    ~WorkThread();
    void AddTask(Task * pt);
    void SetStatus(int s);
private:
    Task* task;
};

inline void WorkThread::AddTask(Task * t) {
    task = t;
    sem_post(this->sem);
}

inline void WorkThread::SetStatus(int s) {
    status = s;
}
#endif

