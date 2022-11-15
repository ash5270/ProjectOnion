#include "ChannelProcess.h"

#include "../Socket/RIOSocket/RIOSession.h"

packet::process::ChannelProcess::ChannelProcess(RIOSessionManager* manager)
{
	this->m_sessionManager = manager;
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
		std::wstring users;
		int32_t count=0;

		for(auto it :*m_sessionManager->GetUserSessionList())
		{
			if(it->userId==L"")
				continue;

			PK_S_NOTIFY_USER_REGISTER notify_user;
			notify_user.id = user_packet->id;
			session->SendPacket(&notify_user);
			users.append(it->userId);
			users.append(L",");
			count++;
		}
		PK_S_ANS_CHANNEL_USERINFO notify_users;
		notify_users.users = users;
		notify_users.user_count = count;
		session->SendPacket(&notify_users);
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
