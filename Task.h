/**
 *	Author: ACb0y
 *	FileName: Task.h
 *	CreateTime: 2013年2月13日23:26:44
 *	Version: 1.0
 */

#ifndef __TASK_LRL_20130213_HEADER__
#define __TASK_LRL_20130213_HEADER__

/*
    抽象任务类
 */
class Task {
    ///属性
public:
    ///nothing.
protected:
    ///nothing.
private:
    ///nothing.

    ///构造函数&&析构函数
public:
    ///nothing.
protected:
    ///nothing.
private:
    ///nothing.

    ///服务
public:
    virtual void * run() = 0;
protected:
    ///nothing.
private:
    ///nothing.
};
#endif

