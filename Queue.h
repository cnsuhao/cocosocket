/* 
 * File:   Queue.h
 * Author: beykery
 *
 * Created on 2014年1月22日, 上午11:22
 */

#ifndef QUEUE_H
#define	QUEUE_H

typedef struct Node {
    void* data;
    Node* next;
} Node;

class Queue {
public:

    Queue() {
    };

    virtual ~Queue() {
    };
    virtual bool Offer(void* e) = 0;
    virtual void* Poll() = 0;
    virtual void* Peek() = 0;
    virtual int Size() = 0;
};

#endif	/* QUEUE_H */

