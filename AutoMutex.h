#ifndef AUTOMUTEX_H
#define AUTOMUTEX_H
#include "Mutex.h"

class AutoMutex {
private:
    Mutex* mutext;
public:

    AutoMutex(Mutex * pMutex) {
        mutext = pMutex;
        mutext->Lock();
    }

    ~AutoMutex() {
        mutext->Unlock();
        delete mutext;
    }
};
#endif

