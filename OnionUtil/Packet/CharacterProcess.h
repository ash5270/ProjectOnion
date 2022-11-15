#pragma once
#include "PacketProcessSystem.h"
#include "../Socket/Session.h"
#include"../Socket/RIOSocket/RIOSessionManager.h"

namespace  onion::packet::process
{
	using socket::RIOSessionManager;
	class CharacterProcess
	{
		RIOSessionManager* m_sessionManager;
	public:
		CharacterProcess(RIOSessionManager* manager);
		void Process(onion::socket::Session* session, Packet* packet);
	};
}