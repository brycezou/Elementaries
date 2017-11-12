package itt.server.multithread;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;


public class MultiThreadServer
{
	public int SERVER_PORT;
	
	public MultiThreadServer(int port)
	{
		SERVER_PORT = port;
	}
	
	public void InitialServer() 
	{  
		ServerSocket serverSocket = null;
		try 
		{  
			serverSocket = new ServerSocket(SERVER_PORT);  
			System.out.println("begin listening...");  
			while (true) 
			{  
				Socket client = serverSocket.accept();  
				System.out.println("one clinet connected!");  
				//一个客户端连接就开户两个线程处理读写  
				new Thread(new ReadHandlerThread(client)).start();  
				new Thread(new WriteHandlerThread(client)).start(); 
			}  
		} 
		catch (Exception e) 
		{  
			e.printStackTrace();  
		} 
		finally
		{
			try 
			{
				if(serverSocket != null)
				{
					serverSocket.close();
				}
			} 
			catch (IOException e) 
			{
				e.printStackTrace();
			}
		}
	}  
}  





