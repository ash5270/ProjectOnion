#include "Socket/IOCPSocket/IOCPClient.h"
#include "System/Buffer.h"

#include"Util/Clock.h"
#include"System/LogSystem.h"

#include<thread>
#include<list>

using namespace onion::util;

int main()
{
	onion::system::LogSystem::getInstance().Start();
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


	
	/*std::list<onion::socket::IOCPClient> clients;
	for(int i=0; i<10;i++)
	{
		clients.emplace_back("127.0.0.1", 3000);
		clients.back().InitializeClient();
		clients.back().StartClient();
	}

	std::wstring msg;

	while (1)
	{
		onion::system::Buffer buf;
		std::getline(std::wcin, msg);
		buf << msg;
		for (auto client : clients)
		{
			client.GetSession()->SendBuffer(&buf);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}*/




	/*onion::system::LogSystem::getInstance().Start();
	Clock::getInstance().StartChecking(1);
	for (int i = 0; i < 10000000; i++)
	{
		
	}

	Clock::getInstance().EndChecking(1);
	onion::system::LogSystem::getInstance().Stop();*/

	onion::system::LogSystem::getInstance().Stop();
	return 0;
}
