#pragma once
#include "../Session.h"
#include "IOCPData.h"

namespace onion::socket
{
	class IOCPSession : public Session 
	{
	public:
		bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo) override;
		void OnSend(size_t transferSize) override;
		void RecvStandBy() override;
		void OnRecv(size_t transferSize) override;
		void OnClose() override;

	public:
		IOCPData* m_data[2];

		IOCPSession();
		~IOCPSession();

	};

}
