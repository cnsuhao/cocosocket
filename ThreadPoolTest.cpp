#include <iostream>
#include "ThreadPool.h"
#include <pthread.h>
#include <sched.h>
using namespace std;

class TestTask : public Task
{
public:

    void * run()
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
    ThreadPool cThreadPool(128, 1);
    TestTask cTest;
    cThreadPool.postTask(&cTest);
    while (true)
    {
        cout << "Current AliveCount = " << cThreadPool.getAliveCount() << endl;
        cThreadPool.postTask(&cTest);
        cout << "Add one task." << endl;
    }
}

