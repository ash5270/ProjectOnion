#include "Socket/IOCPSocket/IOCPClient.h"
#include"Util/Clock.h"
#include<random>
#include<thread>
#include<list>
#include<functional>
#include "Packet/Packet.h"
#include"Socket/IOCPSocket/IOCPDummyClient.h"

#include"PlayerForThread.h"

using namespace onion::util;

int main()
{
	vector<PlayerForThread*> player_threads;

	const int clinet_size =1000;
	onion::socket::IOCPDummyClient dummy("127.0.0.1", 3000);
	dummy.InitializeClient(clinet_size);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	dummy.StartDummyClient();

	int i=0,count=0,check = 0;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	printf_s("start1\n");

	//player_threads.reserve(4);
	player_threads.push_back(new PlayerForThread());

	for (auto session : *dummy.GetSessions()->GetUserSessionList())
	{
		PK_C_REQ_LOGIN login;
		login.id = std::to_wstring(i);
		login.password = L"password";
		auto object = new object::GameObject();
		object->name = login.id;
		session->userHash = std::hash<std::wstring>{}(login.id);
		object->object_id = session->userHash;
		session->SendPacket(&login);
		if (check > clinet_size / 4)
		{
			count++;
			check = 0;
			player_threads.push_back(new PlayerForThread());
		}
		player_threads[count]->sessions.push_back({object,session});
		i++;
		check++;
	}

	//thread 대기 시킴
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	printf_s("start2\n");

	for(int i=0; i< player_threads.size();i++)
	{
		player_threads[i]->Channel();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	for(int i=0; i<player_threads.size(); i++)
	{
		player_threads[i]->Stop();
	}

	dummy.StopClient();
	return 0;
}



