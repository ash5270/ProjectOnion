#include "Socket/IOCPSocket/IOCPClient.h"

int main()
{
	onion::socket::IOCPClient client("127.0.0.1", 3000);
	client.InitializeClient();
	client.StartClient();



	while(1)
	{
		
	}

	return 0;
}
