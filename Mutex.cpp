/**
  *	Author: ACb0y
  *	FileName: Mutex.cpp
  *	CreateTime: 2013年2月4日15:42:15
  *	Version: 1.0
  */


#include "Mutex.h"

Mutex::Mutex(int nShared, int nType)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, nShared);
	pthread_mutexattr_settype(&attr, nType);
	pthread_mutex_init(&m_sMutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&m_sMutex);
}

