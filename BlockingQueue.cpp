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

BlockingQueue::BlockingQueue() : head(NULL), tail(NULL), size(0), lock(NULL)
{
    lock = new Mutext();
}

BlockingQueue::~BlockingQueue()
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

bool BlockingQueue::Offer(void* e)
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

void* BlockingQueue::Peek()
{
    AutoLock l(lock);
    if (head != NULL)
    {
        return head->data;
    }
    return NULL;
}

void* BlockingQueue::Poll()
{
    AutoLock l(lock);
    if (head != NULL)
    {
        void* t = head->data;
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
    return NULL;
}

int BlockingQueue::Size()
{
    AutoLock l(lock);
    return size;
}

