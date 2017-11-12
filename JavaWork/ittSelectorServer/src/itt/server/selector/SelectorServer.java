package itt.server.selector;

import java.io.IOException; 
import java.net.InetSocketAddress; 
import java.nio.ByteBuffer; 
import java.nio.CharBuffer;
import java.nio.channels.SelectionKey; 
import java.nio.channels.Selector; 
import java.nio.channels.ServerSocketChannel; 
import java.nio.channels.SocketChannel; 
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
import java.util.Iterator; 


public class SelectorServer 
{
	private int BUFFER_SIZE = 1024;   
    private ByteBuffer m_clientBuffer = ByteBuffer.allocate(BUFFER_SIZE);   
    private CharsetDecoder m_decoder = null;   
    private CharsetEncoder m_encoder = null;   
    private Selector m_selector = null;   
    
    public SelectorServer(int port)
    {  
        m_decoder = Charset.forName("GB2312").newDecoder();   
        m_encoder = Charset.forName("GB2312").newEncoder();   
        try
        {
	        ServerSocketChannel server = ServerSocketChannel.open();   
	        m_selector = Selector.open();   
	        server.socket().bind(new InetSocketAddress(port));   
	        server.configureBlocking(false);   
	        server.register(m_selector, SelectionKey.OP_ACCEPT);  
        }
        catch (IOException e)
        {
        	e.printStackTrace(); 
        }
    }   
  
    //监听端口   
    public void listen() 
    {   
    	System.out.println("listening...");
        try 
        {   
            while (true) 
            {   
                m_selector.select();   
                Iterator<SelectionKey> iter = m_selector.selectedKeys().iterator();   
                while (iter.hasNext()) 
                {   
                    SelectionKey key = iter.next();   
                    iter.remove();   
                    handleKey(key);   
                }   
            }   
        } 
        catch (IOException e) 
        {   
            e.printStackTrace();   
        }   
    }   
 
    //处理事件   
    private void handleKey(SelectionKey key) throws IOException 
    {   
        if (key.isAcceptable())		//接收请求   
        { 	
            ServerSocketChannel server = (ServerSocketChannel) key.channel();   
            SocketChannel channel = server.accept();   
            //设置非阻塞模式   
            channel.configureBlocking(false);   
            channel.register(m_selector, SelectionKey.OP_READ);   
            System.out.println("one client connected!");
        } 
        else if (key.isReadable())	//读信息 
        { 	  
            SocketChannel channel = (SocketChannel) key.channel();   
            int count = channel.read(m_clientBuffer);   
            if (count > 0) 
            {   
                m_clientBuffer.flip();   
                CharBuffer charBuffer = m_decoder.decode(m_clientBuffer);     
                System.out.println(charBuffer.toString());    
                channel.write(m_encoder.encode(CharBuffer.wrap("welcome"))); 
            } 
            else 
            {   
                channel.close();	//客户已经断开 
            }   
 
            m_clientBuffer.clear();   
        } 
        else if (key.isWritable())	//写事件   
        { 	
            SocketChannel channel = (SocketChannel) key.channel();   
            channel.write(m_encoder.encode(CharBuffer.wrap("**********")));        
        }   
    }   
}
