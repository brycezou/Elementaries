package itt.server.multithread;

public class ServerMain 
{
	public static void main(String[] args) 
	{  
		MultiThreadServer server = new MultiThreadServer(1400);  
		server.InitialServer();  
	}  
}
