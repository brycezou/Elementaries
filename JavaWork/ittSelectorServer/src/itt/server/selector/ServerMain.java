package itt.server.selector;

import java.io.IOException;

public class ServerMain 
{
	public static void main(String[] args) throws IOException  
    { 
        SelectorServer server = new SelectorServer(1400); 
        server.listen();
    }  
}
