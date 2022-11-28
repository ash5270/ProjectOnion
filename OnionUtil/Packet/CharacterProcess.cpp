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

		auto find = m_channel->GetPlayerObject().find(posion_packet->id);
		if(find==m_channel->GetPlayerObject().end())
		{
			delete packet;
			break;
		}
		auto player_object = find->second;
		object::math::Vector3 vec3(posion_packet->pos_x, posion_packet->pos_y, posion_packet->pos_z);
		auto cell = m_channel->GetWorldMap().FindCell(player_object);

		if (!cell->GetRect().BoundRect(vec3))
		{
			m_channel->GetWorldMap().RemoveGameObject(player_object);
			player_object->transform.position = vec3;
			m_channel->GetWorldMap().AddGameObject(player_object);
		}
		else
		{
			player_object->transform.position = vec3;
		}


		object::math::Rect find_rect;
		find_rect.VecToRec(vec3, 10);

		for (auto obj : m_channel->GetWorldMap().FindGameObject(vec3))
		{
			auto session = m_channel->GetUsersSession().find(obj->name);
			if (session == m_channel->GetUsersSession().end())
				break;
			else
				session->second->SendPacket(posion_packet);
		}

		////PO_LOG(LOG_INFO, L"position : {%f,%f,%f}\n", posion_packet->pos_x, posion_packet->pos_y, posion_packet->pos_z);\
		///player_object->transform.position = vec3;
		//for (auto it : *m_sessionManager->GetUserSessionList())
		//{
		//	it->SendPacket(posion_packet);
		//}
		delete packet;
	}
	default:
		break;
	}
}
