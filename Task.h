/* 
 * File:   Task.h
 * Author: beykery
 *
 * Created on 2014年1月15日, 上午11:29
 */

#ifndef TASK_H
#define	TASK_H

class Task {
public:
    Task();
    virtual ~Task();
    virtual void run() = 0;
};
#endif	/* TASK_H */