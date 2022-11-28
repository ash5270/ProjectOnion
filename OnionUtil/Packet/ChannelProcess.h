﻿#pragma once

#include "PacketProcessSystem.h"
#include "../Socket/Session.h"
#include "../System/ChannelSystem.h"
#include"../Socket/RIOSocket/RIOSessionManager.h"


namespace onion::packet::process
{
	using socket::RIOSessionManager;
	class ChannelProcess
	{
		
	public:
		ChannelProcess(RIOSessionManager* manager,Channel* channel);
		~ChannelProcess();

		void Process(onion::socket::Session* session, Packet* packet);
	private:
		Channel* m_channel;
		RIOSessionManager* m_sessionManager;
	};
};
