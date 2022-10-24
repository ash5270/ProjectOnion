﻿#include "RIOSession.h"
#include "../../Util/Common.h"
#include "../../Packet/Packet.h"
#include "../../Packet/PacketAnalyzer.h"

onion::socket::RIOSession::RIOSession(const SOCKET& socket) : Session(socket)
{
	server = nullptr;
}

onion::socket::RIOSession::~RIOSession()
{
	RIOSock::m_Rio_func_table.RIODeregisterBuffer(m_rioBufferRecvID);
	RIOSock::m_Rio_func_table.RIODeregisterBuffer(m_rioBufferSendID);

	VirtualFreeEx(GetCurrentProcess(), m_rioBufferRecvPtr,0, MEM_RELEASE);
	VirtualFreeEx(GetCurrentProcess(), m_rioBufferSendPtr,0, MEM_RELEASE);

	delete m_recvBuffer;
	delete m_sendBuffer;
}

#define TEST_BUFFER_SIZE 40

bool onion::socket::RIOSession::Init()
{
	m_recvBuffer = new system::CircularBuffer(SESSION_BUFFER_SIZE);
	m_sendBuffer = new system::CircularBuffer(SESSION_BUFFER_SIZE);
	m_rioBufferRecvPtr = m_recvBuffer->GetData();
	if(m_rioBufferRecvPtr==nullptr)
	{
		PO_LOG(LOG_ERROR, L"RecvBuffer VirtualAllocEx Error : [%d]\n", GetLastError());
		return false;
	}
	
	m_rioBufferSendPtr = m_sendBuffer->GetData();
	if(m_rioBufferSendPtr==nullptr)
	{
		PO_LOG(LOG_ERROR, L"SendBuffer VirtualAllocEx Error : [%d]\n", GetLastError());
		return false;
	}

	m_rioBufferRecvID = RIOSock::m_Rio_func_table.RIORegisterBuffer(m_rioBufferRecvPtr, SESSION_BUFFER_SIZE);
	if(m_rioBufferRecvID == RIO_INVALID_BUFFERID)
	{
		PO_LOG(LOG_ERROR, L"RIORegister RecvBuffer  Error : [%d]\n", GetLastError());
		return false;
	}

	m_rioBufferSendID = RIOSock::m_Rio_func_table.RIORegisterBuffer(m_rioBufferSendPtr, TEST_BUFFER_SIZE);
	if(m_rioBufferSendID==RIO_INVALID_BUFFERID)
	{
		PO_LOG(LOG_ERROR, L"RIORegister SendBuffer Error : [%d]\n", GetLastError());
		return false;
	}

	//context init
	m_rioSendContext = new RIOContext(this, IO_WRITE, m_rioBufferSendID);
	m_rioRecvContext = new RIOContext(this, IO_READ, m_rioBufferRecvID);

	//버퍼 초기화
	char tmp_char;
	u_long tmp_long = 0;
	long i;
	if (ioctlsocket(m_socket, FIONREAD, &tmp_long) != SOCKET_ERROR)
	{
		for (int i = 0; i < tmp_long; i++)
		{
			recv(m_socket, &tmp_char, sizeof(char), 0);
		}
	}
	if(tmp_long>0)
	{
		printf("recv count %d \n", tmp_long);
	}


	return true;
}

void onion::socket::RIOSession::AddRef()
{
	long ref = InterlockedIncrement(&m_refCount);
	if(ref<0)
	{
		PO_LOG(LOG_ERROR, L"session refcount add error\n");
		return;
	}
}

void onion::socket::RIOSession::ReleaseRef()
{
	long ref = InterlockedDecrement(&m_refCount);
	if (ref < 0)
	{
		PO_LOG(LOG_ERROR, L"session refcount release error\n");
		return;
	}

	if (ref == 0)
	{
		//여기서 세션 삭제
		server->GetSessionManger().ReturnSession(this);
	}
}

long onion::socket::RIOSession::GetRef()
{
	return m_refCount;
}

void onion::socket::RIOSession::SetSocket(const SOCKET& socket)
{
	m_socket = socket;
}

SOCKET& onion::socket::RIOSession::GetSocket() 
{
	return m_socket;
}

void onion::socket::RIOSession::SetThreadID(int threadID)
{
	m_threadID = threadID;
}

bool onion::socket::RIOSession::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	m_socket = socket;
	u_long arg = 1;
	ioctlsocket(m_socket, FIONBIO, &arg);

	int opt = 1;
	setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));

	//create socket rq
	//send랑 recv 를 한 cq에 씀 두개로 만들어서 사용해도 됨
	m_requestQueue = RIOSock::m_Rio_func_table.RIOCreateRequestQueue(m_socket, MAX_RECV_RQ_SIZE_PER_SOCK, 1, MAX_SEND_RQ_SIZE_PER_SOCK, 1
		, server->GetCompletionQueue(m_threadID),server->GetCompletionQueue(m_threadID),NULL);
	if(m_requestQueue==RIO_INVALID_RQ)
	{
		PO_LOG(LOG_ERROR, L"RIO create request queue Error : [%d]\n", GetLastError());
		return false;
	}

	RecvReady();
	return true;
}


void onion::socket::RIOSession::RecvReady()
{
	if (m_recvBuffer->capacity()-m_recvBuffer->offset() <= 0)
		return;
	m_rioRecvContext->BufferId = m_rioBufferRecvID;
	//버퍼에서 남은 자리 
	m_rioRecvContext->Length = static_cast<ULONG>(m_recvBuffer->capacity()-m_recvBuffer->offset());
	m_rioRecvContext->Offset = m_recvBuffer->offset();

	DWORD recvBytes = 0;
	DWORD flags = 0;

	AddRef();

	if (!RIOSock::m_Rio_func_table.RIOReceive(m_requestQueue, (PRIO_BUF)m_rioRecvContext, 1, flags, m_rioRecvContext))
	{
		PO_LOG(LOG_ERROR, L"RIOReceive error : [%d]\n", GetLastError());
		return;
	}
}
Packet* last_p;
size_t last_s;
void onion::socket::RIOSession::OnRecv(size_t transferSize)
{
	//PO_LOG(LOG_DEBUG, L"recv success msg offset : [%d]\n",transferSize);

	//tail쪽을 앞으로 미루어줌 
	m_recvBuffer->HeadCommit(transferSize);
	//앞쪽에 8바이트를 복사해야함 지금은 4바이트만 존재

	int count = transferSize;
	size_t size = 0;


	while(count >= 0)
	{
		auto packet = packet::PacketAnalyzer::getInstance().Analyzer(
			m_recvBuffer->GetData(), m_recvBuffer->tailOffset(),size);
		count -= size + 8;
		if (count < 0)
			break;
		last_p = packet;
		last_s = count;
		//test 
		if(packet->type() == 1 )
		{
			PO_LOG(LOG_DEBUG, L"packet all count : %d\n", packets.size());
			auto last = reinterpret_cast<PK_C_REQ_CHATTING*>(packets.back());
			PO_LOG(LOG_DEBUG, L"packet all last count : %d\n", last->count);

			for (int i = 0; i < packets.size(); i++)
			{
				auto prev = reinterpret_cast<PK_C_REQ_CHATTING*>(packets.front());
				packets.pop();
				auto back = reinterpret_cast<PK_C_REQ_CHATTING*>(packets.front());

				int result = back->count - prev->count;
				if (result > 1)
				{
					printf("d");
				}

				if (result == 0)
				{
					printf("x");
				}
			}

			break;
		}
		if (count < 0)
		{
			break;
		}

		m_recvBuffer->TailCommit(8);
		auto msgPacket = reinterpret_cast<PK_C_REQ_CHATTING*>(packet);
		msgPacket->Deserialize(*m_recvBuffer);
		packets.push(msgPacket);
	/*	PO_LOG(LOG_DEBUG, L"packetId : %ls, msg : %ls\n", msgPacket->id.c_str(), msgPacket->msg.c_str());*/
		//PO_LOG(LOG_DEBUG, L"packetId : %d\n",packet->type());
	}
	m_recvBuffer->Remove(1);
	//PO_LOG(LOG_DEBUG, L"packet all count : %d\n", packets.size());
}

void onion::socket::RIOSession::OnClose()
{
	PO_LOG(LOG_DEBUG, L"packet all count : %d\n", packets.size());
	PO_LOG(LOG_INFO, L"Session OnClose\n");

	PO_LOG(LOG_DEBUG, L"packet all count : %d\n", packets.size());
	auto last = reinterpret_cast<PK_C_REQ_CHATTING*>(packets.back());
	PO_LOG(LOG_DEBUG, L"packet all last count : %d\n", last->count);
	int i = 0;
	for (i = packets.size(); i > 0; i--)
	{
		auto prev = reinterpret_cast<PK_C_REQ_CHATTING*>(packets.front());
		packets.pop();
		auto back = reinterpret_cast<PK_C_REQ_CHATTING*>(packets.front());

		int result = back->count - prev->count;
		if (result > 1)
		{
			printf("d");
		}

		if (result == 0)
		{
			printf("x");
		}
	}

	ReleaseRef();
}

void onion::socket::RIOSession::SendPost()
{
	SpinLockGuard m_guard(lock);
	AddRef();

	//if (m_sendBuffer->GetContiguiousBytes() == 0)
	//	return;

	//m_rioSendContext->BufferId = m_rioBufferSendID;
	////버퍼에서 보낼 길이
	//m_rioSendContext->Length = static_cast<ULONG>(m_sendBuffer->GetContiguiousBytes());
	////버퍼 오프셋
	//m_rioSendContext->Offset = m_sendBuffer->GetReadableOffset();

	DWORD sendBytes = 0;
	DWORD flags = 0;
	if (!RIOSock::m_Rio_func_table.RIOSend(m_requestQueue, (PRIO_BUF)m_rioSendContext, 1, flags, m_rioSendContext))
	{
		PO_LOG(LOG_ERROR, L"RIOSend Error : [%d]\n", GetLastError());
		return;
	}
}

void onion::socket::RIOSession::OnSend(size_t transferSize)
{
	SpinLockGuard m_guard(lock);
	PO_LOG(LOG_INFO, L"send success [%d] \n", transferSize);
	//m_sendBuffer->Remove(transferSize);
}

void onion::socket::RIOSession::SendBuffer(system::Buffer* buffer)
{
	//*m_sendBuffer << buffer;
}

void onion::socket::RIOSession::SendPacket(Packet* packet)
{
	packet->Serialize(*m_sendBuffer);

	
	this->SendPost();
}