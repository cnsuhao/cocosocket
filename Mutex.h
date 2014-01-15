#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>

class Mutex {
private:
    pthread_mutex_t mutext;
public:
    Mutex(int sh = PTHREAD_PROCESS_PRIVATE, int type = PTHREAD_MUTEX_NORMAL);
    ~Mutex();
private:

    Mutex(const Mutex & cMutex) {
    }

    Mutex & operator =(const Mutex & cMutex) {
        return *this;
    }

public:
    int Lock();
    int Unlock();
    int Trylock();
};

inline int Mutex::Lock() {
    return pthread_mutex_lock(&mutext);
}

inline int Mutex::Unlock() {
    return pthread_mutex_unlock(&mutext);
}

inline int Mutex::Trylock() {
    return pthread_mutex_trylock(&mutext);
}

#endif
