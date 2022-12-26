#pragma once
#include <random>

#include "Socket/Session.h"
#include"GameObject/GameObject.h"
#include<vector>
#include<thread>

struct Player
{
	onion::object::GameObject* object;
	onion::socket::Session* session;
};

class PlayerForThread
{
public:
	PlayerForThread();
	~PlayerForThread();

	std::vector<Player> sessions;
	void Channel();
	void Stop();
private:
	std::thread m_thread;
	std::random_device os_seed;
	const int32_t seed;
	std::mt19937 generator;
	void Update();
	void Start();


};
