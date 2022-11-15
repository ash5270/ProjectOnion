#pragma once

#include "PacketProcessSystem.h"
#include "../Socket/Session.h"
#include"../Socket/RIOSocket/RIOSessionManager.h"

namespace onion::packet::process
{
	using socket::RIOSessionManager;
	class LoginProcess
	{
		RIOSessionManager* m_sessionManager;
	public:
		LoginProcess(RIOSessionManager* manager);
		~LoginProcess();

		void Process(onion::socket::Session* session, Packet* packet);
	private:
	};
}
