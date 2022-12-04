#pragma once
#include "../Socket/Session.h"
#include"../Socket/RIOSocket/RIOSessionManager.h"
namespace onion::packet::process
{
	using socket::SessionManager;
	class BasicPacketProcess
	{
	public:
		BasicPacketProcess(SessionManager* manager);
		~BasicPacketProcess();

		void Process(onion::socket::Session* session, Packet* packet);
	private:
		SessionManager* m_sessionManager;
	};
}