#include "WorkThread.h"
#include "errno.h"

WorkThread::WorkThread() : task(NULL)
{
}

WorkThread::~WorkThread()
{
    delete task;
}

void WorkThread::Run()
{
    while (this->status != QUITED)
    {
        if (task != NULL)
        {
            status = RUNNING;
            this->task->Run();
            delete task;
            task = NULL;
        } else
        {
            this->status = IDLE;
            sem_wait(this->sem); //等待添加新的task
        }
    }
}