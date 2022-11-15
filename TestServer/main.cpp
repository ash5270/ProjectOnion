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
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	server.Update();

	while(true)
	{
		
	}
	return 0;
}
