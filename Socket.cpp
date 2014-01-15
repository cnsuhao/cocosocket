/* 
 * File:   Socket.cpp
 * Author: beykery
 * 
 * Created on 2013年12月30日, 下午3:13
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include "Socket.h"
#include "ByteBuf.h"
#include "SocketListerner.h"

Socket::Socket()
{
}

Socket::~Socket()
{
    delete this->ip;
    delete this->listerner;
    delete this->protocal;
}

/**
 * 返回sockeid
 * @return 
 */
int Socket::GetSocketId()
{
    return this->sockid;
}

/**
 * 监听器
 * @return 
 */
SocketListerner* Socket::GetListerner()
{
    return this->listerner;
}

Protocal* Socket::GetProtocal()
{
    return this->protocal;
}

void Socket::SetProtocal(Protocal* p)
{
    this->protocal = p;
}

/**
 * 连接服务器
 * @return 
 */
void Socket::Connect(const char* ip, int port)
{
    this->ip = ip;
    this->port = port;
    struct hostent* p;
    struct sockaddr_in addr;
    p = gethostbyname(this->ip);
    if ((this->sockid = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(this->port);
        addr.sin_addr = *((struct in_addr *) p->h_addr);
        bzero(&(addr.sin_zero), 8);
        if (connect(sockid, (struct sockaddr *) &addr, sizeof (struct sockaddr)) != -1)
        {
            this->listerner->OnOpen(this);
        }
    } else
    {
        this->listerner->OnClose(this, true);
    }
}

/**
 * 发送内容
 * @param frame
 * @return 
 */
int Socket::Send(ByteBuf* frame)
{
    char* content = frame->GetRaw();
    int bytes;
    int count = 0;
    int len = frame->ReadableBytes();
    while (count < len)
    {
        bytes = send(this->sockid, content + count + frame->ReaderIndex(), len - count, 0);
        if (bytes == -1 || bytes == 0)
            return -1;
        count += bytes;
        frame->ReaderIndex(frame->ReaderIndex() + bytes);
    }
    return count;
}

/**
 * 设置一个socket状态监听
 * @param listerner
 */
void Socket::SetListerner(SocketListerner* listerner)
{
    this->listerner = listerner;
    this->listerner->SetContext(this);
}



