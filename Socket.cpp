/* 
 * File:   Socket.cpp
 * Author: beykery
 * 
 * Created on 2013年12月30日, 下午3:13
 */
#include<stdio.h>
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

Socket::Socket()
{
}

Socket::~Socket()
{
    delete this->ip;
    delete this->listerner;
}
Socket* Socket::instance = new Socket();

/**
 *返回唯一实例
 */
Socket* Socket::GetInstance()
{
    return instance;
}

/**
 * 返回sockeid
 * @return 
 */
int Socket::GetSocketId()
{
    return this->sockfd;
}

/**
 * 监听器
 * @return 
 */
SocketListerner* Socket::GetListerner()
{
    return this->listerner;
}

/**
 * 接收线程的id
 */
pthread_t Socket::GetRecvThreadId()
{
    return this->pthread_recv_id;
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
 * 接收消息
 */
static void* Recv(void*)
{
    char buf[1024];
    int sockid = Socket::GetInstance()->GetSocketId();
    while (true)
    {
        int size = recv(sockid, buf, 1024, 0);
        if (size > 0)
        {
            ByteBuf* bb = new ByteBuf(size);
            bb->SetBytes(0, buf, size);
            while (true)
            {
                ByteBuf* frame = Socket::GetInstance()->GetProtocal()->TranslateFrame(bb);
                if (frame != NULL)
                {
                    Socket::GetInstance()->GetListerner()->OnMessage(Socket::GetInstance(), frame);
                } else
                {
                    break;
                }
            }
        } else
        {
            break;
        }
    }
    Socket::GetInstance()->GetListerner()->OnClose(Socket::GetInstance());
}

/**
 * 连接服务器
 * @return 
 */
void Socket::Connect(char* ip, int port)
{
    this->ip = ip;
    this->port = port;
    struct hostent* p;
    struct sockaddr_in addr;
    p = gethostbyname(this->ip);
    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(this->port);
        addr.sin_addr = *((struct in_addr *) p->h_addr);
        bzero(&(addr.sin_zero), 8);
        if (connect(sockfd, (struct sockaddr *) &addr, sizeof (struct sockaddr)) != -1)
        {
            this->listerner->OnOpen(this);
            //启动一个接收线程接收消息
            pthread_t id;
            int ret = pthread_create(&id, NULL, Recv, NULL);
            if (ret != 0)
            {
                this->listerner->OnError(this, "无法创建消息接收线程");
            } else
            {
                this->pthread_recv_id = id;
            }
        }
    }
}

/**
 * 发送消息给服务器
 * @param content
 * @return 
 */
int Socket::Send(char* content, int len)
{
    int bytes;
    int count = 0;
    while (count < len)
    {
        bytes = send(this->sockfd, content + count, len - count, 0);
        if (bytes == -1 || bytes == 0)
            return -1;
        count += bytes;
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
}



