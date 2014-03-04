/* 
 * File:   DefaultListerner.cpp
 * Author: beykery
 * 
 * Created on 2013年12月30日, 下午7:33
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "DefaultListerner.h"
#include "Socket.h"
#include "ByteBuf.h"
#include "Frame.h"
#include "iconv.h"
using namespace std;

DefaultListerner::DefaultListerner()
{
}

DefaultListerner::~DefaultListerner()
{
}

void DefaultListerner::OnClose(Socket* so, bool fromRemote)
{
    cout << "连接关闭" << endl;
}

void DefaultListerner::OnError(Socket* so, const char* e)
{
    cout << "连接错误" << endl;
}

void DefaultListerner::OnIdle(Socket* so)
{
    cout << "连接超时" << endl;
}

/**
 * 有数据到来
 * @param so
 * @param message
 */
void DefaultListerner::OnMessage(Socket* so, ByteBuf* frame)
{
    //    int r = frame->ReadableBytes();
    // cout << frame->Capacity() << endl;
    frame->ReaderIndex(2);
    // setlocale(LC_ALL, "Chinese-simplified");
    //    wchar_t* u1 = frame->ReadUTF8();
    //    wchar_t* u2 = frame->ReadUTF8();
    float c = frame->ReadFloat();
	std::string s=frame->ReadUTF8();
    cout << c << endl;
    delete frame;
    //    Frame* s = new Frame(r);
    //    s->PutInt(c);
    //    s->End();
    //    if (so->Send(s) >= s->GetData()->Capacity())
    //    {
    //        cout << "ok" << endl;
    //    }
    //    delete s;
}

void DefaultListerner::OnOpen(Socket* so)
{
    cout << "连接建立" << endl;
    Frame* f = new Frame(1000);
    f->PutFloat(10.1f);
	char* buff="我去啊a。。。。。";
	f->PutString((char*)ByteBuf::GB23122UTF8(buff).c_str());
	//f->PutString(s);
    f->End();
    so->Send(f);
    delete f;
}