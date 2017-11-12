package itt.client.multithread;

import java.net.Socket;

public class MultiThreadClient 
{
	public MultiThreadClient(String strip, int port)
	{
		try 
		{
			Socket client = new Socket(strip, port);
			//���������̣߳�һ���������һ������д
			new Thread(new ReadHandlerThread(client)).start();
			new Thread(new WriteHandlerThread(client)).start();
			System.out.println("connected!");
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}
}  
