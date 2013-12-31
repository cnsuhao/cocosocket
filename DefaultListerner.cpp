/* 
 * File:   DefaultListerner.cpp
 * Author: beykery
 * 
 * Created on 2013年12月30日, 下午7:33
 */

#include "DefaultListerner.h"
#include "ByteBuf.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

DefaultListerner::DefaultListerner()
{
}

DefaultListerner::~DefaultListerner()
{
}

void DefaultListerner::OnClose(Socket* so)
{
    printf("连接关闭");
}

void DefaultListerner::OnError(Socket* so, char* e)
{
    printf("连接错误");
}

void DefaultListerner::OnIdle(Socket* so)
{
    printf("连接超时");
}

/**
 * 有数据到来
 * @param so
 * @param message
 */
void DefaultListerner::OnMessage(Socket* so, ByteBuf* frame)
{
    printf("有数据到来%d", frame->Capacity());
}

void DefaultListerner::OnOpen(Socket* so)
{
    printf("连接建立");
}



