#pragma once
#include "RIOServer.h"
#include "RIOSock.h"
#include "../Session.h"
#include "../../System/RingBuffer.h"
#include "../../System/Buffer.h"
#include "../../System/SpinLock.h"
#include "RIOSessionManager.h"

class Packet;

namespace onion::socket
{
	class RIOSession : public Session
	{
		volatile long m_refCount;
		
		int m_threadID;

		char* m_rioBufferRecvPtr;
		char* m_rioBufferSendPtr;

		RIO_RQ m_requestQueue;

		RIO_BUFFERID m_rioBufferRecvID;
		RIO_BUFFERID m_rioBufferSendID;

		system::RingBuffer* m_recvBuffer;
		system::RingBuffer* m_sendBuffer;
		SpinLock lock;

		RIOContext* m_rioSendContext;
		RIOContext* m_rioRecvContext;
	public:
		RIOSession(const SOCKET& socket);
		~RIOSession() override;

		//
		bool Init();
		void AddRef();
		void ReleaseRef();
		long GetRef();

		void SetSocket(const SOCKET& socket);
		SOCKET& GetSocket();

		void SetThreadID(int threadID);

		RIOServer* server;
		//
	public:
		bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo) override;
		void OnSend(size_t transferSize) override;
		void RecvReady() override;
		void OnRecv(size_t transferSize) override;
		void OnClose() override;

		void SendPost();
		void SendBuffer(system::Buffer* buffer);
		void SendPacket(Packet* packet);
	};

}

