#include "CharacterProcess.h"
#include<iterator>

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
		}
		else
		{
			player_object->transform.position = vec3;
		}
		delete packet;
	}
	default:
		break;
	}
}

void packet::process::CharacterProcess::Update()
{
	//모든 세션에 위치정보 보냄
	//접속 중인 세션 데이터를 보냄
	for (auto session : m_channel->GetUsersSession())
	{
		//오브젝트 
		auto object = m_channel->GetPlayerObject().find(session.second->userHash);
		if (object == m_channel->GetPlayerObject().end())
			continue;
		//오브젝트가 있는 맵에 있는 유저들만 가져옴
		object::math::Rect find_rect;
		find_rect.VecToRec(object->second->transform.position, 10);
		auto objects = m_channel->GetWorldMap().FindGameObject(object->second->transform.position);

		//데이터를 최대한 맞게 보내기 위해
		size_t packet_size = sizeof(PlayerPos);
		size_t packet_header_size = sizeof(PacketHeader);
		if (objects.size() * packet_size + packet_header_size > BUF_SIZE)
		{
			//오브젝트 즉 플레이어 x 패킷 사이즈가 버퍼 사이즈를 넘어가게 되면
			//오브젝트를 반으로 나누어 보냄
			//만약 이것보다 크면 더 반으로 나눠야 하지만 게임 자체가 그렇게 크지 않기 떄문에 반만 나눔
			//데이터 패킷

			PK_S_NOTIFY_POSION pos_packet;
			pos_packet.user_count = objects.size()/2;
			auto mid = std::next(objects.begin(), objects.size() / 2);
			auto i = objects.begin();

			for (; i != mid; i++)
			{
				PlayerPos player_pos{ static_cast<size_t>((*i)->object_id),
					(*i)->transform.position.x,
					(*i)->transform.position.y,
					(*i)->transform.position.z,
					0,
					0
				};
				pos_packet.player_pos.push_back(player_pos);
			}
			session.second->SendPacket(&pos_packet);

			//두번째
			PK_S_NOTIFY_POSION pos_packet_2;
			pos_packet_2.user_count = objects.size()/2;
			for (; i != objects.end(); i++)
			{
				PlayerPos player_pos{ static_cast<size_t>((*i)->object_id),
					(*i)->transform.position.x,
					(*i)->transform.position.y,
					(*i)->transform.position.z,
					0,
					0
				};
				pos_packet_2.player_pos.push_back(player_pos);
			}
			session.second->SendPacket(&pos_packet_2);
		}
		else
		{

			//플레이어 수 x 패킷 사이즈가 버퍼 사이즈 보다 작을때
			//한번에 뭉쳐서 보낸다.
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
}
