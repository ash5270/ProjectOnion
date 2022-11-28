#pragma once
#include "../Socket/Session.h"
#include"../Socket/RIOSocket/RIOSessionManager.h"
namespace onion::packet::process
{
	using socket::RIOSessionManager;
	class BasicPacketProcess
	{
	public:
		BasicPacketProcess(RIOSessionManager* manager);
		~BasicPacketProcess();

		void Process(onion::socket::Session* session, Packet* packet);
	private:
		RIOSessionManager* m_sessionManager;
	};
}