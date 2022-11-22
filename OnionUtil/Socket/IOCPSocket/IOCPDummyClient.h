#pragma once
#include "IOCPSock.h"
#include <list>
#include"IOCPSessionManager.h"
#include"../../Packet/PacketProcessSystem.h"
namespace onion::socket
{
	class IOCPDummyClient : public IOCPSock
	{
	public:
		IOCPDummyClient(std::string ip, int port);
		~IOCPDummyClient();

		//초기화
		bool InitializeClient(int SessionCount);
		//start
		void StartDummyClient();
		//stop
		void StopClient();

		IOCPSessionManager* GetSessions();
	private:
		SOCKADDR_IN m_serverAddr;
		int m_port;
		std::string m_ip;

		//session manager
		IOCPSessionManager m_sessions;
		PacketProcessSystem* m_packetProcess;
	};
}


