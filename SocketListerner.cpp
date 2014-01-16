/* 
 * File:   SocketListerner.cpp
 * Author: beykery
 * 
 * Created on 2013年12月30日, 下午4:00
 */

#include "SocketListerner.h"
#include "sys/socket.h"
#include "Socket.h"
#include "ByteBuf.h"
#include "Protocal.h"

SocketListerner::SocketListerner()
{
}

SocketListerner::~SocketListerner()
{
}

void SocketListerner::SetContext(Socket* context)
{
    this->context = context;
    this->Start();
}

void SocketListerner::Run()
{
    ByteBuf* buf = new ByteBuf(1024);
    int size = 0;
    int sockid = context->GetSocketId();
    while (true)
    {
        size = recv(sockid, buf->GetRaw(), 1024, 0);
        if (size > 0)
        {
            buf->ReaderIndex(0);
            buf->WriterIndex(size);
            while (true)
            {
                ByteBuf* frame = context->GetProtocal()->TranslateFrame(buf);
                if (frame != NULL)
                {
                    this->OnMessage(context, frame);
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
    delete buf;
    this->OnClose(context, true);
}

