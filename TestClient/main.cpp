#include "Socket/IOCPSocket/IOCPClient.h"
#include "System/Buffer.h"

#include<thread>

int main()
{
	//이 부분 수정해야할듯   

	onion::socket::IOCPClient client("127.0.0.1", 3000);
	client.InitializeClient();
	client.StartClient();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	
	std::wstring msg;

	while(1)
	{
		onion::system::Buffer buf;
		std::getline(std::wcin, msg);
		buf << msg;
		client.GetSession()->SendBuffer(&buf);
	}

	return 0;
}
