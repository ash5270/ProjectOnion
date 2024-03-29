﻿#include "ChannelProcess.h"
#include "../Socket/RIOSocket/RIOSession.h"
#include"../GameObject/OnionMath.h"
packet::process::ChannelProcess::ChannelProcess(SessionManager* manager, Channel* channel)
{
	this->m_sessionManager = manager;
	this->m_channel = channel;
}

void packet::process::ChannelProcess::Process(onion::socket::Session* session, Packet* packet)
{
	switch (packet->type())
	{
		//새로운 유저 입장

	case E_C_REQ_CHANNEL_USERINFO:
	{
		PK_C_REQ_CHANNEL_USERINFO* user_packet = reinterpret_cast<PK_C_REQ_CHANNEL_USERINFO*>(packet);
	
		int32_t count = 0;
		auto obj = new object::GameObject();
		obj->object_id = user_packet->uid;
		obj->transform.position = object::math::Vector3(user_packet->pos_x, user_packet->pos_y, user_packet->pos_z);
		m_channel->AddPlayer(user_packet->uid, obj);
		m_channel->AddPlayerSession(user_packet->uid, session);

		PK_S_NOTIFY_USER_REGISTER notify_user;
		notify_user.id = user_packet->id;
		notify_user.uid = user_packet->uid;
		notify_user.channel_number = 0;
		notify_user.pos_x = user_packet->pos_x;
		notify_user.pos_y = user_packet->pos_y;
		notify_user.pos_z = user_packet->pos_z;

		for (auto it : *m_sessionManager->GetUserSessionList())
		{
			if (it->userId == L"")
				continue;
			
			//새로 들어온 유저 정보 보냄
			it->SendPacket(&notify_user);

			auto obj = m_channel->GetPlayerObject().find(it->userHash);
			if (obj == m_channel->GetPlayerObject().end())
				continue;

			//원래 있던 유저 정보를 지금 요청한 세션에 보냄
			PK_S_ANS_CHANNEL_USERINFO notify_users;
			notify_users.user_count = count;
		
			notify_users.uid = obj->second->object_id;
			notify_users.pos_x = obj->second->transform.position.x;
			notify_users.pos_y = obj->second->transform.position.y;
			notify_users.pos_z = obj->second->transform.position.z;
			notify_users.char_code = 1000;
			session->SendPacket(&notify_users);
		}
	
		delete packet;
		break;
	}
	default:
		break;
	}
}

packet::process::ChannelProcess::~ChannelProcess()
{
}
