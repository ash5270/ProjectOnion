#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "System/LogSystem.h"
#include "System/LogData.h"
#include "Socket/IOCPSocket/IOCPServer.h"

#include "Socket/RIOSocket/RIOServer.h"

int main()
{
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//onion::socket::IOCPServer iocp(3000);
	//iocp.InitializeServer();
	//iocp.StartServer();

	//while(true)
	//{
	//	//update
	//	//server update
	//}

	//iocp.StopServer();


	onion::socket::RIOServer server(4590);
	server.InitializeServer();
	server.StartServer();

	while(true)
	{
		
	}
	return 0;
}
