#include "System/LogSystem.h"
#include "System/LogData.h"
#include "Socket/IOCPSocket/IOCPServer.h"

#include "Socket/RIOSocket/RIOServer.h"

int main()
{
//	onion::socket::RIOServer server(3000);
	onion::socket::IOCPServer server(3000);
	server.InitializeServer();
	server.StartServer();
	//server.Update();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	server.StopServer();
	return 0;
}
