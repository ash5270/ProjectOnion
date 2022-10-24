#pragma once
#include "../Session.h"
#include "IOCPData.h"
#include "../../System/Buffer.h"
#include "../../System/BufferQueue.h"

#include<atomic>

class Packet;

namespace onion::socket
{
	class IOCPSession : public Session 
	{
		//buffer queue send 용
		system::BufferQueue m_buf_queue;
		std::atomic_bool m_is_sending;
		bool m_isConnect;

	public:
		bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo) override;
		void OnSend(size_t transferSize) override;
		//recv를 받을 수 있게 준비 상태로 만들어줌
		void RecvReady() override;
		void OnRecv(size_t transferSize) override;
		void OnClose() override;

		void Send(WSABUF buffer);
		void Recv(WSABUF buffer);

		void SendBuffer(system::Buffer* buffer);
		void SendPacket(Packet* packet);

	public:
		IOCPData* m_data[2];

		IOCPSession(const SOCKET& socket);
		~IOCPSession();

	};

}
