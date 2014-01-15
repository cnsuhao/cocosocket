/* 
 * File:   Socket.h
 * Author: beykery
 *
 * Created on 2013年12月30日, 下午3:13
 */

#ifndef SOCKET_H
#define	SOCKET_H
#include "SocketListerner.h"
#include "Protocal.h"

class Socket {
public:
    Socket();
    virtual ~Socket();
    void Connect(const char* ip, int port);
    int Send(ByteBuf* frame);
    void SetListerner(SocketListerner* listerner);
    void SetProtocal(Protocal* p);
    Protocal* GetProtocal();
    int GetSocketId();
    SocketListerner* GetListerner();
private:
    const char* ip;
    int port;
    int sockid;
    SocketListerner* listerner;
    Protocal* protocal;
};

#endif	/* SOCKET_H */

