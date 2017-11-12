package itt.client.selector;

import java.io.IOException; 
import java.net.InetSocketAddress; 
import java.nio.ByteBuffer; 
import java.nio.CharBuffer; 
import java.nio.channels.SelectionKey; 
import java.nio.channels.Selector; 
import java.nio.channels.SocketChannel; 
import java.nio.charset.Charset; 
import java.nio.charset.CharsetEncoder;
import java.util.Iterator; 


public class SelectorClient 
{ 
	private int BUFFER_SIZE = 1024;   
    private Selector m_selector = null;
    private ByteBuffer m_clientBuffer = ByteBuffer.allocate(BUFFER_SIZE);   
    private CharsetEncoder m_encoder = Charset.forName("GB2312").newEncoder();
    
	public SelectorClient(final String strip, final int port) throws IOException
	{
        final SocketChannel client = SocketChannel.open();   
        //设置为非阻塞模式   
        client.configureBlocking(false);   
        //打开选择器   
        m_selector = Selector.open();   
        //注册连接服务端socket动作   
        client.register(m_selector, SelectionKey.OP_CONNECT);   
        
        new Thread()
    	{
    		@Override
    		public void run()
    		{ 
    			try 
    			{    
        	        client.connect(new InetSocketAddress(strip, port));  
    				_LOOP: while (true)
    				{   
    					m_selector.select();   
    					Iterator<SelectionKey> iter = m_selector.selectedKeys().iterator();   
    					while (iter.hasNext()) 
    					{   
    	                	SelectionKey key = (SelectionKey) iter.next();   
    	                	iter.remove();   
    	                	if (key.isConnectable())
    	                	{   
    	                    	SocketChannel channel = (SocketChannel) key.channel();   
    	                        if (channel.isConnectionPending())   
    	                        	channel.finishConnect();   
    	                        channel.write(m_encoder.encode(CharBuffer.wrap("hellohellohello")));   
    	                        channel.register(m_selector, SelectionKey.OP_READ);   
    	                    } 
    	                	else if (key.isReadable()) 
    	                	{   
    	                		SocketChannel channel = (SocketChannel) key.channel();   
    	                    	int count = channel.read(m_clientBuffer);   
    	                    	String strRecved = "";
    	                    	if (count > 0) 
    	                    	{    
    	                        	m_clientBuffer.flip();   
    	                        	while (m_clientBuffer.remaining() > 0) 
    	                        	{   
    	                            	byte b = m_clientBuffer.get();   
    	                            	strRecved += (char) b;   
    	                        	}   
    	                        	m_clientBuffer.clear();   
    	                        	System.out.println(strRecved);
    	                        } 
    	                    	else 
    	                    	{   
    	                        	client.close();   
    	                            break _LOOP;   
    	                        }   
    	                    }     
    	                }   
    	            } 
    			}
    	        catch (IOException e) 
    	        {   
    	        	e.printStackTrace();   
    	        }   
    		}
    	}.start();
	}
}
