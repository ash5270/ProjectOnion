#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "System/LogSystem.h"
#include "System/LogData.h"
#include "Socket/IOCPSocket/IOCPServer.h"

#include "Socket/RIOSocket/RIOServer.h"

int main()
{
	onion::socket::RIOServer server(3000);
	server.InitializeServer();
	server.StartServer();

	while(true)
	{
		
	}
	return 0;
}
