#include "LoginProcess.h"

#include "../Socket/RIOSocket/RIOSession.h"

packet::process::LoginProcess::LoginProcess(RIOSessionManager* manager)
{
	this->m_sessionManager = manager;
}

packet::process::LoginProcess::~LoginProcess()
{

}

void packet::process::LoginProcess::Process(onion::socket::Session* session, Packet* packet)
{
	switch (packet->type())
	{
	case E_S_ANS_LOGIN:
		printf("recv packet\n");
		break;
	case E_C_REQ_LOGIN:
	{
		PK_C_REQ_LOGIN* login_packet = reinterpret_cast<PK_C_REQ_LOGIN*>(packet);
		PO_LOG(LOG_INFO, L"id : %s, password : %s \n", login_packet->id.c_str(), login_packet->password.c_str());

		session->userId = login_packet->id;

		PK_S_ANS_LOGIN login_result;
		login_result.id = session->userId;
		login_result.result = 400;
		session->SendPacket(&login_result);

		//현제 세션 유저 정보 저장
		std::wstring users_info;
		int count = 0;
		//다른 클라이언트에 로그인 정보 보냄
		for (auto other : *m_sessionManager->GetUserSessionList())
		{
			if ((socket::Session*)other == session)
				continue;
			if (other->userId == L"")
				continue;
			PK_S_NOTIFY_USER_REGISTER  user_info;
			user_info.id = session->userId;
			other->SendPacket(&user_info);
			//나머지 유저 정보 저장
			users_info.append(other->userId);
			users_info.append(L",");
			count++;
		}

		PK_S_ANS_CHANNEL_USERINFO users_info_packets;
		users_info_packets.user_count = count;
		users_info_packets.users = users_info;

		session->SendPacket(&users_info_packets);

		delete packet;
		break;
	}
	default:
		break;
	}
}
