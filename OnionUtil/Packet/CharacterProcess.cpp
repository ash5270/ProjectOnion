#include "CharacterProcess.h"

#include "../Socket/RIOSocket/RIOSession.h"

packet::process::CharacterProcess::CharacterProcess(RIOSessionManager* manager, Channel* channel)
{
	this->m_sessionManager = manager;
	this->m_channel = channel;
}

void packet::process::CharacterProcess::Process(onion::socket::Session* session, Packet* packet)
{
	switch (packet->type())
	{
	case E_C_NOTIFY_POSION:
	{
		PK_C_NOTIFY_POSION* posion_packet = reinterpret_cast<PK_C_NOTIFY_POSION*>(packet);
		m_channel->GetPlayerObject().at(posion_packet->id)->transform.position = object::math::Vector3(posion_packet->pos_x, posion_packet->pos_y, posion_packet->pos_z);
		//PO_LOG(LOG_INFO, L"position : {%f,%f,%f}\n", posion_packet->pos_x, posion_packet->pos_y, posion_packet->pos_z);
		for (auto it : *m_sessionManager->GetUserSessionList())
		{
			it->SendPacket(posion_packet);
		}
		break;
	}
	default:
		break;
	}
}
