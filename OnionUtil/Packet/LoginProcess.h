#pragma once

#include "PacketProcessSystem.h"
#include "../Socket/Session.h"
#include"../Socket/RIOSocket/RIOSessionManager.h"

namespace onion::packet::process
{
	using socket::SessionManager;
	class LoginProcess
	{
		SessionManager* m_sessionManager;
	public:
		LoginProcess(SessionManager* manager);
		~LoginProcess();	

		void Process(onion::socket::Session* session, Packet* packet);
	};
}
