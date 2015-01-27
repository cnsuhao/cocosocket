package org.ngame.socket.test;

import io.netty.bootstrap.Bootstrap;
import io.netty.buffer.Unpooled;
import io.netty.channel.Channel;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.http.DefaultHttpHeaders;
import io.netty.handler.codec.http.HttpClientCodec;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.websocketx.CloseWebSocketFrame;
import io.netty.handler.codec.http.websocketx.PingWebSocketFrame;
import io.netty.handler.codec.http.websocketx.TextWebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketClientHandshakerFactory;
import io.netty.handler.codec.http.websocketx.WebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketVersion;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URI;

public final class WebSocketClient
{

    static final String URL = "ws://127.0.0.1:9999";

    public static void main(String[] args) throws Exception
    {
        URI uri = new URI(URL);
        EventLoopGroup group = new NioEventLoopGroup();
        try
        {
            final WebSocketClientHandler handler = new WebSocketClientHandler(WebSocketClientHandshakerFactory.newHandshaker(uri, WebSocketVersion.V13, null, false, new DefaultHttpHeaders()));
            Bootstrap b = new Bootstrap();
            b.group(group)
                .channel(NioSocketChannel.class)
                .handler(new ChannelInitializer<SocketChannel>()
                    {
                        @Override
                        protected void initChannel(SocketChannel ch)
                        {
                            ChannelPipeline p = ch.pipeline();
                            p.addLast(
                                new HttpClientCodec(),
                                new HttpObjectAggregator(8192),
                                handler);
                        }
                });

            Channel ch = b.connect(uri.getHost(), uri.getPort()).sync().channel();
            handler.handshakeFuture().sync();
            BufferedReader console = new BufferedReader(new InputStreamReader(System.in));
            while (true)
            {
                String msg = console.readLine();
                if (msg == null)
                {
                    break;
                } else if ("bye".equals(msg.toLowerCase()))
                {
                    ch.writeAndFlush(new CloseWebSocketFrame());
                    ch.closeFuture().sync();
                    break;
                } else if ("ping".equals(msg.toLowerCase()))
                {
                    WebSocketFrame frame = new PingWebSocketFrame(Unpooled.wrappedBuffer(new byte[]
                    {
                        8, 1, 8, 1
                    }));
                    ch.writeAndFlush(frame);
                } else
                {
                    WebSocketFrame frame = new TextWebSocketFrame(msg);
                    ch.writeAndFlush(frame);
                }
            }
        } finally
        {
            group.shutdownGracefully();
        }
    }
}
