#include "CharacterProcess.h"

#include "../Socket/RIOSocket/RIOSession.h"

packet::process::CharacterProcess::CharacterProcess(SessionManager* manager, Channel* channel)
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

		auto find = m_channel->GetPlayerObject().find(posion_packet->uid);
		if (find == m_channel->GetPlayerObject().end())
		{
			delete packet;
			break;
		}
		auto player_object = find->second;
		object::math::Vector3 vec3(posion_packet->pos_x, posion_packet->pos_y, posion_packet->pos_z);
		object::math::Vector2 vel(posion_packet->velocity_x, posion_packet->velocity_y);
		player_object->transform.velocity = vel;

		auto cell = m_channel->GetWorldMap().FindCell(player_object);
		if (!cell->GetRect().BoundRect(vec3))
		{
			m_channel->GetWorldMap().RemoveGameObject(player_object);
			player_object->transform.position = vec3;
			m_channel->GetWorldMap().AddGameObject(player_object);
		}else
		{
			player_object->transform.position = vec3;
		}
		/*auto cell = m_channel->GetWorldMap().FindCell(player_object);

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
		}*/

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

void packet::process::CharacterProcess::Update()
{
	//모든 세션에 위치정보 보냄
	for (auto session: m_channel->GetUsersSession())
	{
		//오브젝트 구해옴
		auto object = m_channel->GetPlayerObject().find(session.second->userHash);
		if(object==m_channel->GetPlayerObject().end())
			continue;
		//오브젝트가 있는 맵에 있는 유저들만 가져옴
		object::math::Rect find_rect;
		find_rect.VecToRec(object->second->transform.position, 10);
		auto objects = m_channel->GetWorldMap().FindGameObject(object->second->transform.position);
		//데이터 패킷
		PK_S_NOTIFY_POSION pos_packet;
		pos_packet.user_count = objects.size();
		for (auto obj : objects)
		{
			PlayerPos player_pos{ static_cast<size_t>(obj->object_id),
				obj->transform.position.x,
				obj->transform.position.y,
				obj->transform.position.z,
				0,
				0
			};
			pos_packet.player_pos.push_back(player_pos);
		}
		session.second->SendPacket(&pos_packet);
	}
}


//PK_C_NOTIFY_POSION posion_packet;
//posion_packet.uid = obj->object_id;
//posion_packet.pos_x = obj->transform.position.x;
//posion_packet.pos_y = obj->transform.position.y;
//posion_packet.pos_z = obj->transform.position.z;
//posion_packet.velocity_x = obj->transform.velocity.x;
//posion_packet.velocity_y = obj->transform.velocity.y;
//session.second->SendPacket(&posion_packet);