#include "Socket/IOCPSocket/IOCPClient.h"
#include "System/Buffer.h"

int main()
{
	onion::socket::IOCPClient client("127.0.0.1", 3000);
	client.InitializeClient();
	client.StartClient();


	int x = 012321321;
	onion::system::Buffer buf;
	buf << x;
	client.GetSession()->SendBuffer(&buf);

	

	while(1)
	{
		
	}

	return 0;
}
