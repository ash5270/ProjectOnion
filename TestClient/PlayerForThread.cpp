#include "PlayerForThread.h"

PlayerForThread::PlayerForThread() :seed(os_seed()), generator(seed)
{
}

PlayerForThread::~PlayerForThread()
{
}

void PlayerForThread::Channel()
{
	std::uniform_int_distribution<int32_t> distribution(-70, 70);
	for (int i = 0; i < sessions.size(); i++)
	{
		PK_C_REQ_CHANNEL_USERINFO channel;
		channel.uid = sessions[i].object->object_id;
		channel.channel_number = 1;
		channel.pos_x = distribution(generator);
		channel.pos_y = distribution(generator);
		channel.pos_z = distribution(generator);
		sessions[i].object->transform.position = onion::object::math::Vector3(channel.pos_x, channel.pos_y, channel.pos_z);
		sessions[i].session->SendPacket(&channel);
	}
	m_thread = std::thread(&PlayerForThread::Start, this);
}

void PlayerForThread::Update()
{
	for (int i = 0; i < sessions.size(); i++)
	{
		std::uniform_int_distribution<int32_t> randPos(-5, 5);
		PK_C_NOTIFY_POSION pos;
		pos.uid = sessions[i].object->object_id;
		pos.pos_x = sessions[i].object->transform.position.x + randPos(generator);
		pos.pos_y = sessions[i].object->transform.position.y + randPos(generator);
		pos.pos_z = sessions[i].object->transform.position.z + randPos(generator);
		pos.velocity_x = 0;
		pos.velocity_y = 0;
		sessions[i].session->SendPacket(&pos);
	}
}

void PlayerForThread::Start()
{
	double t = 0.0f;
	const double dt = 1000 / 10;

	auto currentTime = std::chrono::high_resolution_clock::now();
	double accumulator = 0.0f;
	while (true)
	{
		auto newTime = std::chrono::high_resolution_clock::now();
		auto frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(frameTime).count();
		while (accumulator >= dt)
		{
			Update();
			accumulator -= dt;
			t += dt;
		}
	}
}
