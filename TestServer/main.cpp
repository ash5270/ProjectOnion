#include "Socket/IOCPSocket/IOCPServer.h"

int main()
{
	onion::socket::IOCPServer iocp(3000);
	iocp.InitializeServer();
	iocp.StartServer();

	while(true)
	{
		
	}

	iocp.StopServer();
	return 0;
}
