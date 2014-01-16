#include <iostream>
#include "ThreadPool.h"
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
using namespace std;

class TestTask : public Thread
{
public:

    void Run()
    {
        int nCount = 0;
        while (true)
        {
            cout << "[" << ++nCount << "] sleep ..." << endl;
            if (nCount >= 3)
            {
                break;
            }
        }
    }
};

int main()
{
    ThreadPool* cThreadPool = new ThreadPool(5, 1);
    TestTask* cTest = new TestTask;
    cThreadPool->Offer(cTest);
    while (true)
    {
        cout << "Current AliveCount = " << cThreadPool->GetAliveCount() << endl;
        cTest = new TestTask;
        cThreadPool->Offer(cTest);
        cout << "Add one task." << endl;
        sleep(1);
    }
}

