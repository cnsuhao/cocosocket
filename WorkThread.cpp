#include "WorkThread.h"
#include "errno.h"

WorkThread::WorkThread(int nNo) : m_pTask(NULL)
{
}

WorkThread::~WorkThread()
{

}

void WorkThread::run()
{
    this->m_pTask->run();
}
