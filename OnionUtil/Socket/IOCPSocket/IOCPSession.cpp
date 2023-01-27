#include "IOCPSession.h"
#include "cstdio"
#include "../../Packet/PacketFactory.h"
#include "../../System/BufferPool.h"
#include "../../Packet/PacketAnalyzer.h"
#include "../../Packet/PacketObject.h"

onion::socket::IOCPSession::IOCPSession(const SOCKET& socket) : Session(socket), m_recvCount(0)
{
	m_data[0] = new IOCPData();
	m_data[1] = new IOCPData();

	m_socket = socket;

	//IOCP Data에 socket 데이터 등록
	m_data[IO_READ]->SetSocket(m_socket);
	m_data[IO_WRITE]->SetSocket(m_socket);
	//IOCP Data 타입 지정
	m_data[IO_READ]->SetIOType(IO_READ);
	m_data[IO_WRITE]->SetIOType(IO_WRITE);

	m_bufPool = new system::BufferPool();
	m_bufPool->Init(1000,BUF_SIZE);
}

onion::socket::IOCPSession::~IOCPSession()
{
	if (m_data[IO_READ] != nullptr)
	{
		delete m_data[IO_READ];
		m_data[IO_READ] = nullptr;
	}

	if (m_data[IO_WRITE] != nullptr)
	{
		delete m_data[IO_WRITE];
		m_data[IO_WRITE] = nullptr;
	}

	m_bufPool->Delete();
}


bool onion::socket::IOCPSession::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	int opt = 1;
	setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));

	//PO_LOG(LOG_INFO, L"Accpet client\n");
	m_isConnect = true;
	m_isSending.store(false);
	return true;
}
void onion::socket::IOCPSession::OnSend(size_t transferSize)
{
	if (m_bufQueue.empty())
	{
		m_isSending.exchange(false);
		return;
	}

	if (!m_isConnect)
		return;

	m_data[IO_WRITE]->GetBuffer().Clear();

	int size = m_bufQueue.size();
	for (int i = 0; i < size; i++)
	{
		Buffer* send_buf = m_bufQueue.front();
		if (send_buf == nullptr)
		{
			PO_LOG(LOG_INFO, L"[error] send buffer is nullptr\n");
			return;
		}
		if (!(m_data[IO_WRITE]->GetBuffer() << *send_buf))
		{
			//buffer가 가득차서 멈춤
			break;
		}
		m_bufQueue.pop();
		auto pool = m_bufPool;
		pool->Relese(send_buf);
	}

	if (m_data[IO_WRITE]->GetBuffer().size() > 0)
	{
		WSABUF Wsabuf;
		Wsabuf.buf = m_data[IO_WRITE]->GetBuffer().GetData();
		Wsabuf.len = m_data[IO_WRITE]->GetBuffer().offset();
		if (Wsabuf.len <= 0)
			PO_LOG(LOG_INFO, L"WSA BUF size error : %d\n", Wsabuf.len)
		else
			this->Send(Wsabuf);
	}
}


void onion::socket::IOCPSession::OnRecv(size_t transferSize)
{
	m_data[IO_READ]->GetBuffer().HeadCommit(transferSize);
	//PO_LOG(LOG_INFO, L"[info] recv size : [%d]\n", transferSize);

	int count = transferSize + m_recvCount;
	int headerSize = sizeof(PacketHeader);
	size_t packet_size = 0;

	while (count >= 0)
	{
		CircularBuffer* buf = &m_data[IO_READ]->GetBuffer();
		auto packet = packet::PacketAnalyzer::getInstance().Analyzer(
			buf->GetData(), buf->tailOffset(), packet_size); 
		m_recvCount = count;
		count -= packet_size + headerSize;

		if (count < 0)
		{
			break;
		}
		buf->TailCommit(headerSize);
		packet->Deserialize(m_data[IO_READ]->GetBuffer());

		PacketObject* obj = new PacketObject();
		obj->session = this;
		obj->packet = packet;

		m_packet_process_system->PushPacket(obj);
	}
	m_data[IO_READ]->GetBuffer().Remove(1);
}

void onion::socket::IOCPSession::OnClose()
{
	m_isConnect = false;
	PO_LOG(LOG_INFO, L"[info]  disconnect\n");
}

void onion::socket::IOCPSession::Send(WSABUF buffer)
{
	if (buffer.len <= 0)
	{
		PO_LOG(LOG_ERROR, L"IOCP send buffer error : %d\n", buffer.len)
			return;
	}

	int nResult = 0;
	DWORD sendBytes;
	nResult = WSASend(this->m_socket, &buffer, 1, &sendBytes, 0, m_data[IO_WRITE]->GetOverlapped(), NULL);
	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		PO_LOG(LOG_ERROR, L"WSASend falied : %d\n", WSAGetLastError());
	}
}

void onion::socket::IOCPSession::Recv(WSABUF buffer)
{
	int nResult = 0;
	DWORD recvBytes = 0;
	DWORD flags = 0;
	nResult = WSARecv(this->m_socket, &buffer, 1, &recvBytes, &flags, m_data[IO_READ]->GetOverlapped(), NULL);
	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		PO_LOG(LOG_ERROR, L"IO pending failed : %d\n", WSAGetLastError());
	}
}

void onion::socket::IOCPSession::SendBuffer(system::Buffer* buffer)
{
	if (m_data[IO_WRITE]->GetIOType() != IO_WRITE)
		return;

	m_data[IO_WRITE]->Clear();
	WSABUF buf;
	buf.buf = buffer->GetData();
	buf.len = buffer->size();

	this->Send(buf);
}

void onion::socket::IOCPSession::SendPacket(Packet* packet)
{
	if (!m_isConnect)
		return;

	auto pool = m_bufPool;
	auto buf = pool->GetBuffer();
	auto header = packet->Serialize(*buf);
	header->size = buf->write_size;
	m_bufQueue.push_back(buf);

	bool sending = false;
	if (m_isSending.compare_exchange_strong(sending, true))
	{
		if(m_bufQueue.empty())
			return;

		m_data[IO_WRITE]->Clear();
		auto send_buf = m_bufQueue.front();
		m_bufQueue.pop();
		m_data[IO_WRITE]->GetBuffer() << *send_buf;
		WSABUF Wsabuf;
		Wsabuf.buf = m_data[IO_WRITE]->GetBuffer().GetData();
		Wsabuf.len = m_data[IO_WRITE]->GetBuffer().offset() - m_data[IO_WRITE]->GetBuffer().tailOffset();

		this->Send(Wsabuf);

		//system::BufferPool::getInstance().Relese(buf);
		m_bufPool->Relese(buf);
	}
}

void onion::socket::IOCPSession::RecvReady()
{
	//수정해야할곳 
	//m_data[IO_READ]->Clear();
	WSABUF buf;
	buf.buf = m_data[IO_READ]->GetBuffer().GetData() + m_data[IO_READ]->GetBuffer().offset();
	buf.len = m_data[IO_READ]->GetBuffer().capacity() - m_data[IO_READ]->GetBuffer().offset();

	this->Recv(buf);
}

