#include "LoginProcess.h"
#include "../Socket/RIOSocket/RIOSession.h"
#include <functional>

packet::process::LoginProcess::LoginProcess(SessionManager* manager)
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
		session->userHash = std::hash<std::wstring>{}(session->userId);
		PK_S_ANS_LOGIN login_result;
		login_result.id = session->userId;
		login_result.uid = session->userHash;
		login_result.result = 400;
		//밑에 부분은 채널 시스템에서 따로 실행
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
			user_info.uid = session->userHash;
			other->SendPacket(&user_info);
			//나머지 유저 정보 저장

			/*PK_S_ANS_CHANNEL_USERINFO users_info_packets;
			users_info_packets.user_count = count;
			users_info_packets.users = other->userId;
			users_info_packets.

			session->SendPacket(&users_info_packets);*/
			count++;
		}

		delete packet;
		break;
	}
	default:
		break;
	}
}
