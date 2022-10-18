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
	std::random_device dr;
	onion::system::LogSystem::getInstance().Start();
	//이 부분 수정해야할듯   

	onion::socket::IOCPClient client("127.0.0.1", 4590);
	client.InitializeClient();
	client.StartClient();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	PK_C_REQ_CHATTING packet;
	packet.id = L"ash527ttttttttttttttt1";
	packet.msg = L"testttttttttttttttttttt2";

	/*client.GetSession()->SendPacket(&packet);
	client.GetSession()->SendPacket(&packet);
	client.GetSession()->SendPacket(&packet);*/

	int x= 0;

	auto end = chrono::steady_clock::now();
	PO_LOG(LOG_DEBUG, L"%s\n", Clock::getInstance().NowTime(TIME_FORMAT).c_str());
	for(int k=0; k<3; k++)
	{
		auto start = chrono::steady_clock::now();
		x = 0;
		for (int i = 0; i < 100000000; i++)
		{
			x++;
			end = chrono::steady_clock::now();
			auto re = chrono::duration_cast<std::chrono::seconds>(end - start).count();

			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			client.GetSession()->SendPacket(&packet);
			if (re > 1)
			{
				break;
			}
		}
		printf("i= %d\n", x);
	}
	



	while(1)
	{
		
	}

	//auto re = chrono::duration_cast<std::chrono::seconds>(end - start).count();

	//while (1)
	//{
	//	
	//	std::this_thread::sleep_for(std::chrono::microseconds(1));
	//	printf(" %d\n", re);
	//	PO_LOG(LOG_DEBUG, L"%d\n", re);

	//	client.GetSession()->SendPacket(&packet);
	//}
	//printf("i : %d\n",x);



	client.StopClient();

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
			client.GetSession()->SendPacket(&buf);
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

	//onion::system::CircularBuffer buffer(1024);

	//std::mt19937 gen(dr());

	//std::uniform_int_distribution<int> dis(0, 120000);

	//for(int i=0; i< 1000; i++)
	//{
	//	int a = dis(gen);
	//	int b = dis(gen);
	//	int c = dis(gen);
	//	int a1 = 0;
	//	int b1 = 0;
	//	int c1 = 0;

	//	buffer.Write(reinterpret_cast<char*>(&a), sizeof(a));

	//	buffer.Read(reinterpret_cast<void*>(&a1), sizeof(a1));

	//	buffer.Write(reinterpret_cast<char*>(&b), sizeof(b));
	//	buffer.Write(reinterpret_cast<char*>(&c), sizeof(c));

	//	
	//	buffer.Read(reinterpret_cast<void*>(&b1), sizeof(b1));
	//	buffer.Read(reinterpret_cast<void*>(&c1), sizeof(c1));

	//	printf_s("input[%d, %d, %d] output [%d,%d,%d]\n", a, b, c, a1, b1, c1);
	//}

	/*int x = 123;
	float z = 0.12321;
	float c = 1123213;
	int v = 123123;
	int b = 00001;

	int x1 = 0;
	float z1 = 0;
	float c1 = 0;
	int v1 = 0;
	int b1 = 0;

	buffer.Write(reinterpret_cast<char*>(&x), sizeof(x));
	buffer.Write(reinterpret_cast<char*>(&z), sizeof(z));
	buffer.Write(reinterpret_cast<char*>(&c), sizeof(c));
	buffer.Write(reinterpret_cast<char*>(&v), sizeof(v));
	buffer.Read(reinterpret_cast<void*>(&x1), sizeof(x1));
	buffer.Write(reinterpret_cast<char*>(&b), sizeof(b));



	buffer.Read(reinterpret_cast<void*>(&z1), sizeof(z1));
	buffer.Read(reinterpret_cast<void*>(&c1), sizeof(c1));
	buffer.Read(reinterpret_cast<void*>(&v1), sizeof(v1));
	buffer.Read(reinterpret_cast<void*>(&b1), sizeof(b1));*/


	onion::system::LogSystem::getInstance().Stop();
	return 0;
}
