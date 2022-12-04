#pragma once
#include "PacketProcessSystem.h"
#include "../Socket/Session.h"
#include"../Socket/RIOSocket/RIOSessionManager.h"
#include"../System/ChannelSystem.h"

namespace  onion::packet::process
{
	using socket::SessionManager;
	class CharacterProcess
	{
		SessionManager* m_sessionManager;
	public:
		CharacterProcess(SessionManager* manager, Channel* channel);
		void Process(onion::socket::Session* session, Packet* packet);
		void Update();
	private:
		Channel* m_channel;
	};
}