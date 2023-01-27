#include "RIOSession.h"
#include "../../Util/Common.h"
#include "../../System/Buffer.h"
#include "../../Packet/Packet.h"
#include "../../Packet/PacketAnalyzer.h"
#include "../../Packet/PacketObject.h"

#include"../../System/BufferPool.h"

onion::socket::RIOSession::RIOSession(const SOCKET& socket) : Session(socket)
{
	server = nullptr;
	m_bufPool = new system::BufferPool();
	m_bufPool->Init(1000,BUF_SIZE);
}

onion::socket::RIOSession::~RIOSession()
{
	RIOSock::m_Rio_func_table.RIODeregisterBuffer(m_rioBufferRecvID);
	RIOSock::m_Rio_func_table.RIODeregisterBuffer(m_rioBufferSendID);

	VirtualFreeEx(GetCurrentProcess(), m_rioBufferRecvPtr, 0, MEM_RELEASE);
	VirtualFreeEx(GetCurrentProcess(), m_rioBufferSendPtr, 0, MEM_RELEASE);

	m_bufPool->Delete();

	delete m_recvBuffer;
	delete m_sendBuffer;
}

#define TEST_BUFFER_SIZE 40

bool onion::socket::RIOSession::Init()
{
	m_recvBuffer = new system::CircularBuffer(SESSION_BUFFER_SIZE);
	m_sendBuffer = new system::CircularBuffer(SESSION_BUFFER_SIZE);
	m_rioBufferRecvPtr = m_recvBuffer->GetData();
	if (m_rioBufferRecvPtr == nullptr)
	{
		PO_LOG(LOG_ERROR, L"RecvBuffer VirtualAllocEx Error : [%d]\n", GetLastError());
		return false;
	}

	m_rioBufferSendPtr = m_sendBuffer->GetData();
	if (m_rioBufferSendPtr == nullptr)
	{
		PO_LOG(LOG_ERROR, L"SendBuffer VirtualAllocEx Error : [%d]\n", GetLastError());
		return false;
	}

	m_rioBufferRecvID = RIOSock::m_Rio_func_table.RIORegisterBuffer(m_rioBufferRecvPtr, SESSION_BUFFER_SIZE);
	if (m_rioBufferRecvID == RIO_INVALID_BUFFERID)
	{
		PO_LOG(LOG_ERROR, L"RIORegister RecvBuffer  Error : [%d]\n", GetLastError());
		return false;
	}

	m_rioBufferSendID = RIOSock::m_Rio_func_table.RIORegisterBuffer(m_rioBufferSendPtr, SESSION_BUFFER_SIZE);
	if (m_rioBufferSendID == RIO_INVALID_BUFFERID)
	{
		PO_LOG(LOG_ERROR, L"RIORegister SendBuffer Error : [%d]\n", GetLastError());
		return false;
	}

	//context init
	m_rioSendContext = new RIOContext(this, IO_WRITE, m_rioBufferSendID);
	m_rioRecvContext = new RIOContext(this, IO_READ, m_rioBufferRecvID);

	m_recvCount = 0;
	m_isSending.store(false);

	m_isConnect = true;
	return true;
}

void onion::socket::RIOSession::AddRef()
{
	long ref = InterlockedIncrement(&m_refCount);
	if (ref < 0)
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
		, server->GetCompletionQueue(m_threadID), server->GetCompletionQueue(m_threadID), NULL);
	if (m_requestQueue == RIO_INVALID_RQ)
	{
		PO_LOG(LOG_ERROR, L"RIO create request queue Error : [%d]\n", GetLastError());
		return false;
	}

	RecvReady();
	return true;
}


void onion::socket::RIOSession::RecvReady()
{
	if (m_recvBuffer->capacity() - m_recvBuffer->offset() <= 0)
		return;
	m_rioRecvContext->BufferId = m_rioBufferRecvID;
	//버퍼에서 남은 자리 
	m_rioRecvContext->Length = static_cast<ULONG>(m_recvBuffer->capacity() - m_recvBuffer->offset());
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


void onion::socket::RIOSession::OnRecv(size_t transferSize)
{
	//tail쪽을 앞으로 미루어줌
	m_recvBuffer->HeadCommit(transferSize);
	int count = transferSize + m_recvCount;
	int headerSize = sizeof(PacketHeader);
	size_t size = 0;
	//데이터가 다 받아졌는지 확인
	while (count >= 0)
	{
		auto packet = packet::PacketAnalyzer::getInstance().Analyzer(
			m_recvBuffer->GetData(), m_recvBuffer->tailOffset(), size);
		m_recvCount = count;
		count -= size + headerSize;

		if (count < 0)
			break;
		//PO_LOG(LOG_INFO, L"recv : packet id : %d, size : %d\n", packet->type(), size);
		m_recvBuffer->TailCommit(headerSize);
		packet->Deserialize(*m_recvBuffer);

		PacketObject* obj = new PacketObject();
		obj->session = this;
		obj->packet = packet;

		m_packet_process_system->PushPacket(obj);
	}

	m_recvBuffer->Remove(1);
}

void onion::socket::RIOSession::OnClose()
{
	m_isConnect = false;
	PO_LOG(LOG_INFO, L"Session OnClose\n");
	ReleaseRef();
}

void onion::socket::RIOSession::SendPost()
{
	AddRef();

	m_rioSendContext->BufferId = m_rioBufferSendID;
	//버퍼에서 보낼 길이
	m_rioSendContext->Length = static_cast<ULONG>(m_sendBuffer->offset());
	//버퍼 오프셋
	m_rioSendContext->Offset = m_sendBuffer->tailOffset();

	if (m_rioSendContext->Length <= 0)
	{
		PO_LOG(LOG_ERROR, L"rio send buffer error : %d\n", m_rioSendContext->Length)
			return;
	}

	DWORD flags = 0;
	if (!RIOSock::m_Rio_func_table.RIOSend(m_requestQueue, (PRIO_BUF)m_rioSendContext, 1, flags, m_rioSendContext))
	{
		PO_LOG(LOG_ERROR, L"RIOSend Error : [%d]\n", GetLastError());
		ReleaseRef();
		return;
	}
}

void onion::socket::RIOSession::OnSend(size_t transferSize)
{
	if (m_bufQueue.empty())
	{
		m_isSending.exchange(false);
		return;
	}

	if (!m_isConnect)
		return;

	m_sendBuffer->Clear();
	int size = m_bufQueue.size();
	for (int i = 0; i <size; i++)
	{
		Buffer* send_buf = m_bufQueue.front();
		if (send_buf == nullptr)
		{
			PO_LOG(LOG_INFO, L"[error] send buffer is nullptr\n");
			return;
		}
		if (!(*m_sendBuffer << *send_buf))
		{
			break;
		}
		m_bufQueue.pop();
		auto pool = m_bufPool;
		pool->Relese(send_buf);
	}

	if (m_sendBuffer->size() > 0)
	{
		this->SendPost();
	}
}

void onion::socket::RIOSession::SendBuffer(system::Buffer* buffer)
{
	//*m_sendBuffer << buffer;
}

void onion::socket::RIOSession::SendPacket(Packet* packet)
{
	if (!m_isConnect)
		return;

	auto pool = m_bufPool;
	auto buf = pool->GetBuffer();
	auto header = packet->Serialize(*buf);
	header->size = buf->write_size;

	m_bufQueue.push_back(buf);

	bool check = false;
	if (m_isSending.compare_exchange_strong(check, true))
	{
		if (m_bufQueue.empty())
			return;

		m_sendBuffer->Clear();
		m_bufQueue.pop();
		*m_sendBuffer << *buf;
		pool->Relese(buf);
		//onion::system::BufferPool::getInstance().Relese(buf);
		this->SendPost();
	}
}