#pragma once
#include "../Session.h"
#include "IOCPData.h"
#include "../../System/Buffer.h"


namespace onion::socket
{
	class IOCPSession : public Session 
	{
	public:
		bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo) override;
		void OnSend(size_t transferSize) override;
		void RecvReady() override;
		void OnRecv(size_t transferSize) override;
		void OnClose() override;

		void Send(WSABUF buffer);
		void Recv(WSABUF buffer);

		void SendBuffer(system::Buffer* buffer);

	public:
		IOCPData* m_data[2];

		IOCPSession();
		~IOCPSession();

	};

}
