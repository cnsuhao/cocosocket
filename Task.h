#ifndef TASK_H
#define TASK_H

/*
    抽象任务类
 */
class Task {
public:
    virtual void * run() = 0;
};
#endif

