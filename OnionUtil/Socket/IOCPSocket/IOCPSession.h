///IOCP Session
///이곳에서 I/O데이터를 받은것을 처리
///
///

#pragma once
#include "../Session.h"
#include "IOCPData.h"
#include "../../System/Buffer.h"
#include "../../System/BufferQueue.h"
#include"../../Packet/PacketProcessSystem.h"
#include"../../System/BufferPool.h"

class Packet;

namespace onion::socket
{
	class IOCPSession : public Session 
	{
	private:
		//buffer queue send 용
		system::BufferQueue m_bufQueue;
		std::atomic_bool m_isSending;
		bool m_isConnect;
		int m_recvCount;

		//buffer pool
		system::BufferPool* m_bufPool;

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
		void SendPacket(Packet* packet) override;

	public:
		IOCPData* m_data[2];

		IOCPSession(const SOCKET& socket);
		~IOCPSession();

	};

}
