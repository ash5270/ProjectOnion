#include "PacketProcessSystem.h"
#include "PacketObject.h"

PacketProcessSystem::PacketProcessSystem() : m_isUpdate(false), m_packetQueue()
{

}

PacketProcessSystem::~PacketProcessSystem()
{
	m_isUpdate = false;
}


void PacketProcessSystem::Start()
{
	m_isUpdate = true;
	m_thread = std::thread(&PacketProcessSystem::Loop, this);
	PO_LOG(LOG_INFO, L"PacketProcess Start...\n");
}

void PacketProcessSystem::Process(PacketObject* packetObject)
{
	auto func = m_packetTable.find(packetObject->packet->type());
	if (func == m_packetTable.end())
		return;
	func->second(packetObject->session, packetObject->packet);
}
int fps_count = 0;

void PacketProcessSystem::Update()
{
	if (m_packetQueue.empty())
	{
		return;
	}

	for (int i = 0; i < m_packetQueue.size(); i++)
	{
		PacketObject* obj;
		bool check = m_packetQueue.try_Dequeue(obj);
		if (check)
			Process(obj);
		else
			continue;
	}
	if(fps_count>3)
	{
		for (int i = 0; i < m_funcs.size(); i++)
		{
			m_funcs[i]();
		}
		fps_count = 0;
	}
	fps_count++;
}

void PacketProcessSystem::Loop()
{
	double t = 0.0f;
	const double dt = 1000 / 30;

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

void PacketProcessSystem::Stop()
{
	m_isUpdate = false;
	if (m_thread.joinable())
		m_thread.join();
}



