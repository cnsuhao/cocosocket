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
#include <stdlib.h>
#include "AutoLock.h"

typedef struct Node {
    void* data;
    Node* next;
} Node;

template <class T>
class BlockingQueue : public Queue<T> {
public:

    BlockingQueue() : head(NULL), tail(NULL), size(0), lock(NULL) {
        lock = new Mutext();
    };

    virtual~BlockingQueue() {
        delete this->lock;
        Node* p = head;
        while (p != NULL) {
            delete ((T*) p->data);
            Node* t = p;
            p = p->next;
            free(t);
        }
    };

    bool Offer(T* e) {
        AutoLock l(lock);
        Node* p = (Node*) malloc(sizeof (Node));
        p->data = e;
        p->next = NULL;
        if (size <= 0) {
            head = p;
            tail = p;
        } else {
            tail->next = p;
            tail = p;
        }
        size++;
        return true;
    };

    T* Peek() {
        AutoLock l(lock);
        if (head != NULL) {
            return (T*) (head->data);
        }
        return NULL;
    };

    T* Poll() {
        AutoLock l(lock);
        if (head != NULL) {
            T* t = (T*) (head->data);
            Node* oh = head;
            head = head->next;
            if (head == NULL) {
                tail = NULL;
            }
            free(oh);
            size--;
            return t;
        }
        return (T*) (NULL);
    };

    int Size() {
        AutoLock l(lock);
        return size;
    };
private:
    Node* head;
    Node* tail;
    int size;
    Mutext* lock;
};

#endif	/* BLOCKINGQUEUE_H */

