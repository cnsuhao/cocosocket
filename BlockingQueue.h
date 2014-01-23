/* 
 * File:   BlockingQueue.h
 * Author: beykery
 *
 * Created on 2014年1月22日, 上午11:39
 */

#ifndef BLOCKINGQUEUE_H
#define	BLOCKINGQUEUE_H

#include "Mutext.h"
#include "Queue.h"

template <class T>
class BlockingQueue : public Queue<T> {
public:
    BlockingQueue();
    virtual ~BlockingQueue();
    bool Offer(T* e);
    T* Peek();
    T* Poll();
    int Size();
private:
    Node* head;
    Node* tail;
    int size;
    Mutext* lock;
};

#endif	/* BLOCKINGQUEUE_H */

