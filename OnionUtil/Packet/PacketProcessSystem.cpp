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
	m_thread = std::thread(&PacketProcessSystem::Update, this);
	PO_LOG(LOG_INFO, L"PacketProcess Start...\n");
}

void PacketProcessSystem::Process(PacketObject* packetObject)
{
	auto func= m_packetTable.at(packetObject->packet->type());
	func(packetObject->session,packetObject->packet);
}

void PacketProcessSystem::Update()
{
	while(m_isUpdate)
	{
		if(m_packetQueue.empty())
			continue;

		PacketObject* obj;
		bool check = m_packetQueue.try_Dequeue(obj);
		if (check)
			Process(obj);
		else
			continue;
	}
}

void PacketProcessSystem::Stop()
{
	m_isUpdate = false;
	if (m_thread.joinable())
		m_thread.join();
}



