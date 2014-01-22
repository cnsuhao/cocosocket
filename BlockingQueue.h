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

class BlockingQueue : public Queue {
public:
    BlockingQueue();
    virtual ~BlockingQueue();
    bool Offer(void* e);
    void* Peek();
    void* Poll();
    int Size();
private:
    Node* head;
    Node* tail;
    int size;
    Mutext* lock;
};

#endif	/* BLOCKINGQUEUE_H */

