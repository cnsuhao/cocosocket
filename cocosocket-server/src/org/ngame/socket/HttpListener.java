/**
 * http监听
 */
package org.ngame.socket;

import io.netty.buffer.ByteBuf;
import io.netty.handler.timeout.IdleStateEvent;

/**
 *
 * @author beykery
 */
public abstract class HttpListener
{

    public abstract void onOpen(NHttp conn);

    public abstract void onClose(NHttp conn, boolean local);

    public abstract void onMessage(NHttp conn, ByteBuf message);

    public abstract void onError(NHttp conn, Throwable ex);

    public abstract void onIdle(NHttp conn, IdleStateEvent event);
}
