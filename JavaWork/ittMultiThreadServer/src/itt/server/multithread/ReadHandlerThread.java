package itt.server.multithread;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;

/*
 *������������߳� 
 */
class ReadHandlerThread implements Runnable
{
	private Socket client;
	public ReadHandlerThread(Socket client) 
	{
		this.client = client;
	}

	@Override
	public void run() 
	{
		DataInputStream dis = null;
		try
		{
			dis = new DataInputStream(client.getInputStream());
			while(true)
			{
				String reciver = dis.readUTF();
				System.out.println("�ͻ��˷�����������: " + reciver); 
			}
		}
		catch(Exception e)
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
				if(client != null)
				{
					client.close();
					client = null;
				}
			} 
			catch (IOException e) 
			{
				e.printStackTrace();
			}
		}
	}
}