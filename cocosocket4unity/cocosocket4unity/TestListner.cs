using System;
using LitJson;

namespace cocosocket4unity
{
	public class TestListner : SocketListner
	{
		public TestListner ()
		{
		}

		public override  void OnMessage(USocket us,ByteBuf bb)
		{
			Console.WriteLine ("收到数据:");
			bb.ReaderIndex (us.getProtocal().HeaderLen());
			string s=bb.ReadUTF8 ();
			Console.WriteLine (s);
			bb.ReaderIndex (0);
			us.Send (bb);//原样返回给服务器，服务器是一个echo，也会原样返回
		}
		/**
		 * 
		 */ 
		public override  void OnClose(USocket us,bool fromRemote)
		{
			Console.WriteLine ("连接被关闭："+fromRemote);
		}
		public override  void OnIdle(USocket us)
		{
			Console.WriteLine ("连接超时：");
		}
		public override  void OnOpen(USocket us)
		{
			Console.WriteLine ("连接建立");
			JsonData data = new JsonData ();
			data ["cmd"] = 1;
			data ["name"] = "你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好";
			data ["pwd"] = "ldfkjl你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好";
			data ["test"] = "asdlkj;fal;sdflads很合地方sdflads很合地方sdflads很合地方sdflads很合地方sdflads很合地方sdflads很合地方sdflads很合地方sdflads很合地方sdflads很合地方sdflads很合地方sdflads很合地方sdflads很合地方";
			Varint32Frame f = new Varint32Frame (512);
			f.PutString (data.ToJson());
			f.End ();
			us.Send (f);
		}
		public override  void OnError(USocket us,string err)
		{
			Console.WriteLine ("错误："+err);
		}
	}
}

