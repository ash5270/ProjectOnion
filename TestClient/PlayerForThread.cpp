#include "PlayerForThread.h"

PlayerForThread::PlayerForThread():seed(os_seed()),generator(seed)
{
}

PlayerForThread::~PlayerForThread()
{
}

void PlayerForThread::Channel()
{
	std::uniform_int_distribution<int32_t> distribution(-100, 100);
	for (int i=0; i<sessions.size(); i++)
	{
		PK_C_REQ_CHANNEL_USERINFO channel;
		channel.id = sessions[i].object->name;
		channel.channel_number = 1;
		channel.pos_x = distribution(generator);
		channel.pos_y = distribution(generator);
		channel.pos_z = distribution(generator);
		sessions[i].object->transform.position = onion::object::math::Vector3(channel.pos_x, channel.pos_y, channel.pos_z);
		sessions[i].session->SendPacket(&channel);
	}
	m_thread = std::thread(&PlayerForThread::Update,this);
}

void PlayerForThread::Update()
{
	std::uniform_int_distribution<int32_t> randPos(-5, 5);
	while (true)
	{
		for (int i = 0; i < sessions.size(); i++)
		{
			PK_C_NOTIFY_POSION pos;
			pos.id = sessions[i].object->name;
			pos.pos_x = sessions[i].object->transform.position.x + randPos(generator);
			pos.pos_y = sessions[i].object->transform.position.y + randPos(generator);
			pos.pos_z = sessions[i].object->transform.position.z + randPos(generator);
			pos.velocity_x = 0;
			pos.velocity_y = 0;
			sessions[i].session->SendPacket(&pos);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
