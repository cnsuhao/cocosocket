/**
 * 进程内的本地server
 */
package org.ngame.socket;

import java.net.InetSocketAddress;
import java.net.UnknownHostException;

/**
 *
 * @author beykery
 */
public abstract class LocalServer extends NServer
{

	public LocalServer(InetSocketAddress addr) throws UnknownHostException
	{
		super(addr, NETWORK_LOCAL);
	}
}
