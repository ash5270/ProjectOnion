#pragma once
#include "PacketProcessSystem.h"
#include "../Socket/Session.h"
#include"../Socket/RIOSocket/RIOSessionManager.h"
#include"../System/ChannelSystem.h"

namespace  onion::packet::process
{
	using socket::RIOSessionManager;
	class CharacterProcess
	{
		RIOSessionManager* m_sessionManager;
	public:
		CharacterProcess(RIOSessionManager* manager, Channel* channel);
		void Process(onion::socket::Session* session, Packet* packet);
	private:
		Channel* m_channel;
	};
}