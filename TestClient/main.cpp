#include "Socket/IOCPSocket/IOCPClient.h"
#include "System/Buffer.h"

#include"Util/Clock.h"
#include"System/LogSystem.h"

#include<random>

#include<thread>
#include<list>

#include "Packet/PacketFactory.h"
#include "Packet/Packet.h"


using namespace onion::util;

int main()
{
	onion::system::LogSystem::getInstance().Start();
	onion::socket::IOCPClient client("127.0.0.1", 3000);
	client.InitializeClient();
	client.StartClient();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	PK_C_REQ_CHATTING packet;
	packet.id = L"테스트";
	packet.msg = L"테스트 메시지";

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
			packet.count = i;
			client.GetSession()->SendPacket(&packet);
			if (re > 1000)
			{
				PO_LOG(LOG_INFO, L"send packet count : %d\n", i);
				break;
			}
		}
		
	}

	PK_S_ANS_CHATTING end_packet;
	end_packet.id = L"1";
	end_packet.msg = L"1";

	client.GetSession()->SendPacket(&end_packet);

	while(1)
	{
		
	}

	onion::system::LogSystem::getInstance().Stop();
	return 0;
}
