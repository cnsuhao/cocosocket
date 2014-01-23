/* 
 * File:   BlockingQueue.cpp
 * Author: beykery
 * 
 * Created on 2014年1月22日, 上午11:39
 */

#include <stdlib.h>
#include "BlockingQueue.h"
#include "stddef.h"
#include "AutoLock.h"
#include "Mutext.h"
#include "Queue.h"

template <class T>
BlockingQueue<T>::BlockingQueue() : head(NULL), tail(NULL), size(0), lock(NULL)
{
    lock = new Mutext();
}

template <class T>
BlockingQueue<T>::~BlockingQueue()
{
    delete this->lock;
    Node* p = head;
    while (p != NULL)
    {
        delete p->data;
        Node* t = p;
        p = p->next;
        free(t);
    }
}

template <class T>
bool BlockingQueue<T>::Offer(T* e)
{
    AutoLock l(lock);
    Node* p = (Node*) malloc(sizeof (Node));
    p->data = e;
    p->next = NULL;
    if (size <= 0)
    {
        head = p;
        tail = p;
    } else
    {
        tail->next = p;
        tail = p;
    }
    size++;
    return true;
}

template <class T>
T* BlockingQueue<T>::Peek()
{
    AutoLock l(lock);
    if (head != NULL)
    {
        return (T*) (head->data);
    }
    return NULL;
}

template <class T>
T* BlockingQueue<T>::Poll()
{
    AutoLock l(lock);
    if (head != NULL)
    {
        T* t = (T*) (head->data);
        Node* oh = head;
        head = head->next;
        if (head == NULL)
        {
            tail = NULL;
        }
        free(oh);
        size--;
        return t;
    }
    return (T*) (NULL);
}

template <class T>
int BlockingQueue<T>::Size()
{
    AutoLock l(lock);
    return size;
}

