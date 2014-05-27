/**
 * 支持http协议的服务器
 */
package org.ngame.socket;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.buffer.PooledByteBufAllocator;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.http.HttpServerCodec;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.ngame.socket.protocal.NullProtocal;
import org.ngame.socket.protocal.Protocal;

/**
 *
 * @author beykery
 */
public abstract class HttpServer extends HttpListener
{

    private static final Logger LOG = Logger.getLogger(SocketServer.class.getName());
    protected static final int DEFAULT_PORT = 8888;
    private int port = DEFAULT_PORT;
    private InetSocketAddress address;
    private Channel serverChannel;
    private EventLoopGroup bossGroup;
    private EventLoopGroup workerGroup;
    private static int MAX_THREAD_SELECTOR = 2;
    private static int MAX_THREAD_IO = Runtime.getRuntime().availableProcessors() * 2;
    private Class<? extends Protocal> pClass = NullProtocal.class;

    static
    {
        try
        {
            MAX_THREAD_SELECTOR = Integer.parseInt(System.getProperty("game.socket.server.thread.selector"));
            MAX_THREAD_IO = Integer.parseInt(System.getProperty("game.socket.server.thread.io"));
        } catch (Exception e)
        {
            LOG.log(Level.WARNING, "服务器配置信息错误 " + e.getMessage());
        }
    }

    public HttpServer() throws UnknownHostException
    {
        this(null);
    }

    /**
     * 构造
     *
     * @param address
     */
    public HttpServer(InetSocketAddress address)
    {
        if (address == null)
        {
            /**
             * 端口号
             */
            try
            {
                String sport = System.getProperty("game.server.port");
                port = Integer.parseInt(sport);
            } catch (Exception e)
            {
                LOG.log(Level.WARNING, "服务器端口配置错误，将使用默认端口");
            }
            address = new InetSocketAddress(port);
        } else
        {
            port = address.getPort();
        }
        setAddress(address);
    }

    private void setAddress(InetSocketAddress address)
    {
        this.address = address;
    }

    public InetSocketAddress getAddress()
    {
        return address;
    }

    /**
     * 设置分帧处理器
     *
     * @param pClass
     */
    public void setProtocal(Class<? extends Protocal> pClass)
    {
        this.pClass = pClass;
    }

    /**
     * 查询分帧逻辑类
     *
     * @return
     */
    public Class<? extends Protocal> getProtocal()
    {
        return pClass;
    }

    /**
     * 启动服务器
     *
     * @throws InterruptedException
     */
    public void start() throws InterruptedException
    {
        final Class c;
        bossGroup = new NioEventLoopGroup(MAX_THREAD_SELECTOR);
        workerGroup = new NioEventLoopGroup(MAX_THREAD_IO);
        c = NioServerSocketChannel.class;
        ServerBootstrap b = new ServerBootstrap();
        b.group(bossGroup, workerGroup).
            channel(c).
            option(ChannelOption.TCP_NODELAY, true).
            option(ChannelOption.CONNECT_TIMEOUT_MILLIS, 0).
            option(ChannelOption.ALLOCATOR, PooledByteBufAllocator.DEFAULT).
            childHandler(new ChannelInitializer<SocketChannel>()
                {
                    @Override
                    public void initChannel(SocketChannel ch) throws Exception
                    {
                        ChannelPipeline p = ch.pipeline();
                        p.addLast("codec", new HttpServerCodec());
                        p.addLast("handler", new NHttp(HttpServer.this, ch));
                        
                        
                    }
            });
        final ChannelFuture f = b.bind(port).sync();
        this.serverChannel = f.channel();
    }

    /**
     * 停止服务器
     *
     * @return
     */
    public ChannelFuture stop()
    {
        this.preStop();
        workerGroup.shutdownGracefully();
        bossGroup.shutdownGracefully();
        return this.serverChannel.close();
    }

    /**
     * 停止服务器前需要做的事情
     */
    protected abstract void preStop();
}
