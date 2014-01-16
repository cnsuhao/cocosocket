#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <semaphore.h>
#include "Thread.h"

class WorkThread : public Thread {
protected:
    void Run();
public:
    WorkThread();
    ~WorkThread();
    void AddTask(Thread * pt);
    void SetStatus(int s);
private:
    Thread* task;
};

inline void WorkThread::AddTask(Thread * t) {
    task = t;
    sem_post(this->sem);
}

inline void WorkThread::SetStatus(int s) {
    status = s;
}
#endif

