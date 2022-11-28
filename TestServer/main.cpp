#include "System/LogSystem.h"
#include "System/LogData.h"
#include "Socket/IOCPSocket/IOCPServer.h"

#include "Socket/RIOSocket/RIOServer.h"

int main()
{
	onion::socket::RIOServer server(3000);
	server.InitializeServer();
	server.StartServer();
	server.Update();
	server.StopServer();
	return 0;
}
