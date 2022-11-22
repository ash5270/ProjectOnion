#include "ChannelProcess.h"
#include "../Socket/RIOSocket/RIOSession.h"

packet::process::ChannelProcess::ChannelProcess(RIOSessionManager* manager, Channel* channel)
{
	this->m_sessionManager = manager;
	this->m_channel = channel;
}

void packet::process::ChannelProcess::Process(onion::socket::Session* session, Packet* packet)
{
	switch (packet->type())
	{
		//새로운 유저 입장
		//
	case E_C_REQ_CHANNEL_USERINFO:
	{
		PK_C_REQ_CHANNEL_USERINFO* user_packet = reinterpret_cast<PK_C_REQ_CHANNEL_USERINFO*>(packet);
	
		int32_t count = 0;
		auto obj = new object::GameObject();
		obj->name = user_packet->id;	
		m_channel->AddPlayer(user_packet->id, obj);

		for (auto it : *m_sessionManager->GetUserSessionList())
		{
			if (it->userId == L"")
				continue;
			//새로 들어온 유저 정보 보냄
			PK_S_NOTIFY_USER_REGISTER notify_user;
			notify_user.id = user_packet->id;
			session->SendPacket(&notify_user);

			//원래 있던 유저 정보를 지금 요청한 세션에 보냄
			PK_S_ANS_CHANNEL_USERINFO notify_users;
			notify_users.user_count = count;
			auto obj = m_channel->GetPlayerObject().find(it->userId);
			if(obj==m_channel->GetPlayerObject().end())
				continue;
			notify_users.users = obj->second->name;
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
