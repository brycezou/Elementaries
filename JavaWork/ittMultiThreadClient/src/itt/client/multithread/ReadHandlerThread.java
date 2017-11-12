package itt.client.multithread;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;

/*
 *处理读操作的线程 
 */
class ReadHandlerThread implements Runnable
{
	private Socket m_client;
	public ReadHandlerThread(Socket client) 
	{
		this.m_client = client;
	}

	@Override
	public void run() 
	{
		DataInputStream dis = null;
		try 
		{
			dis = new DataInputStream(m_client.getInputStream());
			while(true)
			{
				String receive = dis.readUTF();   
				System.out.println("服务器端返回过来的是: " + receive);  
			}
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		} 
		finally
		{
			try 
			{
				if(dis != null)
				{
					dis.close();
					dis = null;
				}
				if(m_client != null)
				{
					m_client.close();
					m_client = null;
				}
			} 
			catch (IOException e) 
			{
				e.printStackTrace();
			}
		}
	}
}
