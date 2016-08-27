﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace cocosocket4unity
{
    /// <summary>
    /// kcp客戶端程序
    /// </summary>
   public class KcpClient : KcpOnUdp
    {
       private LinkedList<ByteBuf> sendList;
       private bool running;
       
       public KcpClient(int port):base(port)
       {
           this.sendList = new LinkedList<ByteBuf>();
       }
       public void Stop()
       {
           running = false;
       }
       /// <summary>
       /// 开启线程开始工作
       /// </summary>
       public void Start()
       {
           if (!this.running)
           {
               this.running = true;
               Thread t = new Thread(new ThreadStart(run));//启动发送线程，同步发送
               t.IsBackground = true;
               t.Start();
           }
       }
       private void run()
       {
           while (running)
           {
               DateTime st = DateTime.Now;
               this.Update();
               lock (this.sendList)
               {
                   while(this.sendList.Count>0)
                   {
                       ByteBuf bb=this.sendList.First.Value;
                       sendList.RemoveFirst();
                       this.kcp.Send(bb);
                   }
               }
               if (this.needUpdate)
               {
                   continue;
               }
               DateTime end = DateTime.Now;
               while ((end - st).TotalMilliseconds < 10)
               {
                   if (this.needUpdate) 
                   {
                       break;                         
                   }
                   Thread.Yield();
                   end = DateTime.Now;
               }
           }
       }
      /// <summary>
      /// 处理udp的消息
      /// </summary>
      /// <param name="bb"></param>
       protected override void HandleReceive(ByteBuf bb) 
       {
          string s = System.Text.Encoding.UTF8.GetString(bb.GetRaw(),0,bb.ReadableBytes());
          Console.WriteLine("收到消息: "+s);
          this.Send(bb);//回送
       }
       public void Send(ByteBuf content)
       {
           lock (this.sendList)
           {
               this.sendList.AddLast(content);
               this.needUpdate = true;
           }
       }
       /// <summary>
       /// 測試
       /// </summary>
       /// <param name="args"></param>
       public static void Main(string[] args)
       {
           KcpClient client = new KcpClient(2222);
           client.NoDelay(1, 10, 2, 1);//fast
           client.WndSize(64, 64);
           client.Connect("10.18.121.15",2222);
           client.Start();
           Thread.Sleep(2000);
           String s = "hi,heoll world! 你好啊！！";
           for (int i = 0; i < 10; i++)
           {
               s = s + s;
           }
           ByteBuf bb = new ByteBuf(System.Text.Encoding.UTF8.GetBytes(s));
           Console.WriteLine(bb.ReadableBytes());
           client.Send(bb);
           Console.Read();
       }
    }
}