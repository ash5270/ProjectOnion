#include "Socket/IOCPSocket/IOCPClient.h"
#include "System/Buffer.h"

#include"Util/Clock.h"
#include"System/LogSystem.h"

#include<random>

#include<thread>
#include<list>

#include "Packet/PacketFactory.h"
#include "Packet/Packet.h"

#include"Socket/IOCPSocket/IOCPDummyClient.h"

using namespace onion::util;

int main()
{
	/*onion::system::LogSystem::getInstance().Start();
	onion::socket::IOCPClient client("127.0.0.1", 3000);
	client.InitializeClient();
	client.StartClient();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	PK_C_REQ_CHATTING packet;
	packet.id = L"123213213213213";
	packet.msg = L"sadsadsadsads";

	int x= 0;
	auto end = chrono::steady_clock::now();
	for(int k=0; k<3; k++)
	{
		auto start = chrono::steady_clock::now();
		x = 0;
		for (int i = 0; i < 100000000; i++)
		{
			x++;
			end = chrono::steady_clock::now();
			auto re = chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			
			client.GetSessions()->SendPacket(&packet);
			if (re >= 1000)
			{
				PO_LOG(LOG_INFO, L"send packet count : %d , time : %d\n", i, re);
				break;
			}
		}
	}

	PK_S_ANS_CHATTING end_packet;
	end_packet.id = L"1";
	end_packet.msg = L"1";

	client.GetSessions()->SendPacket(&end_packet);

	while(1)
	{
		
	}

	onion::system::LogSystem::getInstance().Stop();*/

	onion::socket::IOCPDummyClient dummy("127.0.0.1", 3000);
	dummy.InitializeClient(3000);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	dummy.StartDummyClient();

	int i = 0;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	printf_s("start1\n");
	for (auto session : dummy.GetSessions()->GetSessionList())
	{
		PK_C_REQ_LOGIN login;
		login.id = std::to_wstring(i);
		login.password = L"password";
		session->SendPacket(&login);
		i++;
	}
	i = 0;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	printf_s("start2\n");
	for (auto session : dummy.GetSessions()->GetSessionList())
	{
		PK_C_REQ_CHANNEL_USERINFO channel;
		channel.id = std::to_wstring(i);
		channel.channel_number = 1;
		session->SendPacket(&channel);
		i++;
	}


	while (true)
	{
		
	}

	dummy.StopClient();
	return 0;
}
