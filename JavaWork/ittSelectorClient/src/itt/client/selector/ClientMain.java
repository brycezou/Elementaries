package itt.client.selector;

import java.io.IOException;

public class ClientMain 
{
	public static void main(String[] args) throws IOException 
	{
		@SuppressWarnings("unused")
		SelectorClient client = new SelectorClient("192.168.23.1", 1400);
	}

}
