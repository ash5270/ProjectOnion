#include "IOCPSession.h"
#include "cstdio"
#include "../../Packet/PacketFactory.h"

onion::socket::IOCPSession::IOCPSession(const SOCKET& socket) : Session(socket)
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
}

onion::socket::IOCPSession::~IOCPSession()
{
	if(m_data[IO_READ]!=nullptr)
	{
		delete m_data[IO_READ];
		m_data[IO_READ] = nullptr;
	}

	if (m_data[IO_WRITE] != nullptr)
	{
		delete m_data[IO_WRITE];
		m_data[IO_WRITE] = nullptr;
	}
}


bool onion::socket::IOCPSession::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	int opt = 1;
	setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));

	PO_LOG(LOG_INFO, L"Accpet client\n");
	m_isConnect = true;
	m_is_sending.store(false);
	return true;
}
void onion::socket::IOCPSession::OnSend(size_t transferSize)
{
	if(m_buf_queue.empty())
	{
		m_is_sending.exchange(false);
		return;
	}

	if (!m_isConnect)
		return;

	m_data[IO_WRITE]->GetBuffer().Clear();

	for(int i=0; i < m_buf_queue.size(); i++)
	{
		Buffer* send_buf = m_buf_queue.front();
		if (send_buf == nullptr)
		{
			PO_LOG(LOG_INFO, L"[error] send buffer is nullptr\n");
			return;
		}
		if(!(m_data[IO_WRITE]->GetBuffer()<<*send_buf))
		{
			//buffer가 가득차서 멈춤
			break;
		}
		delete send_buf;
		m_buf_queue.pop();
	}

	if (m_data[IO_WRITE]->GetBuffer().size() > 0)
	{
		WSABUF Wsabuf;
		Wsabuf.buf = m_data[IO_WRITE]->GetBuffer().GetData();
		Wsabuf.len = m_data[IO_WRITE]->GetBuffer().offset();
		this->Send(Wsabuf);
	}
}


void onion::socket::IOCPSession::OnRecv(size_t transferSize)
{
	m_data[IO_READ]->GetBuffer().HeadCommit(transferSize);
	PO_LOG(LOG_INFO, L"[info] recv size : [%d]\n", transferSize);
	//Session::OnRecv();
}

void onion::socket::IOCPSession::OnClose()
{
	m_isConnect = false;
	PO_LOG(LOG_INFO, L"[info]  disconnect\n");
}

void onion::socket::IOCPSession::Send(WSABUF buffer)
{
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
	Buffer* buf = new Buffer(128);
	auto header = packet->Serialize(*buf);
	header->size = buf->write_size;
	
	m_buf_queue.push_back(buf);
	bool sending = false;
	////보내고 있는 동안 여기 와서 에러 나는거 같은데??
	if(m_is_sending.compare_exchange_strong(sending, true))
	{
		m_data[IO_WRITE]->Clear();
		m_buf_queue.pop();
		m_data[IO_WRITE]->GetBuffer() << *buf;
		WSABUF Wsabuf;
		Wsabuf.buf = m_data[IO_WRITE]->GetBuffer().GetData();
		Wsabuf.len = m_data[IO_WRITE]->GetBuffer().offset()-m_data[IO_WRITE]->GetBuffer().tailOffset();
		this->Send(Wsabuf);
	}
}
	
void onion::socket::IOCPSession::RecvReady()
{
	//수정해야할곳 
	m_data[IO_READ]->Clear();
	WSABUF buf;
	buf.buf = m_data[IO_READ]->GetBuffer().GetData();
	buf.len = m_data[IO_READ]->GetBuffer().capacity();

	this->Recv(buf);
}

