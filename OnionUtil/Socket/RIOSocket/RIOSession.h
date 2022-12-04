#pragma once
#include "RIOServer.h"
#include "RIOSock.h"
#include "../Session.h"
#include "../../System/CircularBuffer.h"
#include "../../System/Buffer.h"
#include "../../System/SpinLock.h"
#include "../../System/BufferQueue.h"
#include "../../System/BufferPool.h"
#include "RIOSessionManager.h"

class Packet;

namespace onion::socket
{
	class RIOSession : public Session
	{
		volatile long m_refCount;
		int m_threadID;
		int m_recvCount;

		char* m_rioBufferRecvPtr;
		char* m_rioBufferSendPtr;

		RIO_RQ m_requestQueue;

		RIO_BUFFERID m_rioBufferRecvID;
		RIO_BUFFERID m_rioBufferSendID;

		system::CircularBuffer* m_recvBuffer;
		system::CircularBuffer* m_sendBuffer;
		SpinLock lock;

		RIOContext* m_rioSendContext;
		RIOContext* m_rioRecvContext;

		//send buffer queue
		//모아서 보내는 용
		system::BufferQueue m_bufQueue;
		system::BufferQueue m_packtes;
		//queue lock
		std::atomic_bool m_isSending;
		bool m_isConnect;
		//buffer pool
		system::BufferPool* m_bufPool;
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
		void SendPacket(Packet* packet) override;

		
	};

}

