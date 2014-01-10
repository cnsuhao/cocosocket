/**
 *	Author: ACb0y
 *	FileName: Thread.cpp
 *	CreateTime: 2013年2月13日14:16:20
 *	Version: 1.0
 */

#include "Thread.h"

CThread::CThread() : m_nThreadId(NULL)
{
    ///nothing.
}

CThread::~CThread()
{
    ///nothing.
}

int CThread::create(pFuncThreadStart pFuncStartRoutine, void * pArg, bool bDetached, bool bSetScope)
{
    pthread_attr_t sThread_attr;
    int nStatus;
    nStatus = pthread_attr_init(&sThread_attr);
    if (nStatus != 0)
    {
        return -1;
    }

    if (bDetached)
    {
        nStatus = pthread_attr_setdetachstate(&sThread_attr, PTHREAD_CREATE_DETACHED);
        if (nStatus != 0)
        {
            pthread_attr_destroy(&sThread_attr);
            return -1;
        }
    }

    if (bSetScope)
    {
        nStatus = pthread_attr_setscope(&sThread_attr, PTHREAD_SCOPE_SYSTEM);
        if (nStatus != 0)
        {
            pthread_attr_destroy(&sThread_attr);
            return -1;
        }
    }

    nStatus = pthread_create(&m_nThreadId, &sThread_attr, pFuncStartRoutine, pArg);
    pthread_attr_destroy(&sThread_attr);
    return nStatus;
}



