#include "Socket/IOCPSocket/IOCPClient.h"
#include"Util/Clock.h"
#include<random>
#include<thread>
#include<list>
#include "Packet/Packet.h"
#include"Socket/IOCPSocket/IOCPDummyClient.h"

#include"GameObject/WorldMap.h"
#include"PlayerForThread.h"

using namespace onion::util;

int main()
{


	/*object::QuadTree tree(onion::object::math::Vector2(-300, 300), onion::object::math::Vector2(300, -300));

	object::GameObject obj2;
	obj2.transform.position = object::math::Vector3(-250, 300, 1);

	tree.Insert(&obj2);

	object::math::Rect rect;
	rect.leftTop = object::math::Vector2(-300, 300);
	rect.rightBtm = object::math::Vector2(300, -300);
	std::list<object::GameObject*> objs;
	tree.GetGameObject(objs, rect);
	tree.Remove(&obj2);

	std::cout << "d";
	*/
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

	vector<PlayerForThread*> player_threads;

	const int clinet_size = 500;
	onion::socket::IOCPDummyClient dummy("127.0.0.1", 3000);
	dummy.InitializeClient(clinet_size);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	dummy.StartDummyClient();

	int i=0,count=0,check = 0;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	printf_s("start1\n");

	//player_threads.reserve(4);
	player_threads.push_back(new PlayerForThread());

	for (auto session : dummy.GetSessions()->GetSessionList())
	{
		PK_C_REQ_LOGIN login;
		login.id = std::to_wstring(i);
		login.password = L"password";
		auto object = new object::GameObject();
		object->name = login.id;
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

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	printf_s("start2\n");

	for(int i=0; i< player_threads.size();i++)
	{
		player_threads[i]->Channel();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	}

	while (true)
	{
		
	}

	/*for (auto session : dummy.GetSessions()->GetSessionList())
	{
		PK_C_REQ_CHANNEL_USERINFO channel;
		channel.id = std::to_wstring(i);
		channel.channel_number = 1;
		channel.pos_x = distribution(generator);
		channel.pos_y = distribution(generator);
		channel.pos_z = distribution(generator);
		gameObjects[i]->transform.position = object::math::Vector3(channel.pos_x, channel.pos_y, channel.pos_z);
		session->SendPacket(&channel);
		i++;
	}
	i = 0;
	std::uniform_int_distribution<int32_t> randPos(-5, 5);
	while (true)
	{
		for (auto session : dummy.GetSessions()->GetSessionList())
		{
			PK_C_NOTIFY_POSION pos;
			if (i >= clinet_size)
				i = 0;

			pos.id = std::to_wstring(i);
			pos.pos_x = gameObjects[i]->transform.position.x + randPos(generator);
			pos.pos_y = gameObjects[i]->transform.position.y + randPos(generator);
			pos.pos_z = gameObjects[i]->transform.position.z + randPos(generator);
			pos.velocity_x = 0;
			pos.velocity_y = 0;
			session->SendPacket(&pos);
			i++;
		}
		this_thread::sleep_for(std::chrono::milliseconds(300));
	}*/

	dummy.StopClient();
	return 0;
}



