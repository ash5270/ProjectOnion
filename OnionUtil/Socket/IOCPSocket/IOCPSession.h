#pragma once
#include "../Session.h"
#include "IOCPData.h"

namespace OnionSocket
{
	class IOCPSession : public Session
	{
	protected:
		bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo) override;
		void Send(size_t transferSize) override;
		void RecvStandBy() override;
		void OnRecv() override;
		void OnClose() override;
	public:
		IOCPData* m_data[2];

		IOCPSession();
		~IOCPSession();

	};

}
