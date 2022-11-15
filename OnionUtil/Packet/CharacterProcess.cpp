#include "CharacterProcess.h"

#include "../Socket/RIOSocket/RIOSession.h"

packet::process::CharacterProcess::CharacterProcess(RIOSessionManager* manager)
{
	this->m_sessionManager = manager;
}

void packet::process::CharacterProcess::Process(onion::socket::Session* session, Packet* packet)
{
	switch (packet->type())
	{
	case E_C_NOTIFY_POSION:
	{
		PK_C_NOTIFY_POSION* posion_packet = reinterpret_cast<PK_C_NOTIFY_POSION*>(packet);
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
