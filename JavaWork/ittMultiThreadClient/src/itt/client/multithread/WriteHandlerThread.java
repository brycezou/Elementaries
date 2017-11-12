package itt.client.multithread;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

/*
 * ����д�������߳�
 */
class WriteHandlerThread implements Runnable
{
	private Socket m_client;
	public WriteHandlerThread(Socket m_client) 
	{
		this.m_client = m_client;
	}

	@Override
	public void run() 
	{
		DataOutputStream dos = null;
		BufferedReader br = null;
		try 
		{
			dos = new DataOutputStream(m_client.getOutputStream());
			br = new BufferedReader(new InputStreamReader(System.in));
			while(true)
			{
				System.out.println("������:");  
				String send = br.readLine();  
				//��������
				dos.writeUTF(send);  
				dos.flush();
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
				if(dos != null)
				{
					dos.close();
					dos = null;
				}
				if(br != null)
				{
					br.close();
					br = null;
				}
				if(m_client != null)
				{
					m_client.close();
					m_client = null;
				}
			}catch(Exception e)
			{
				e.printStackTrace();
			}
		}
	}
}
