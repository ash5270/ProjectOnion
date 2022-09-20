#include "IOCPSession.h"
#include "cstdio"

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
	PO_LOG(LOG_INFO, L"Accpet client\n");
	return false;
}

void onion::socket::IOCPSession::OnSend(size_t transferSize)
{
	PO_LOG(LOG_INFO, L"[info] send size : [%d]\n", transferSize);
}


void onion::socket::IOCPSession::OnRecv(size_t transferSize)
{
	PO_LOG(LOG_INFO, L"[info] recv size : [%d]\n", transferSize);
	std::wstring x;
	m_data[IO_READ]->GetBuffer() >> &x;
	PO_LOG(LOG_DEBUG, L"recv msg : [%s]\n", x.c_str());
	//Session::OnRecv();
}

void onion::socket::IOCPSession::OnClose()
{
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
	nResult = WSARecv(m_data[IO_READ]->GetSocket(), &buffer, 1, &recvBytes, &flags, m_data[IO_READ]->GetOverlapped(), NULL);
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

void onion::socket::IOCPSession::RecvReady()
{
	//수정해야할곳 
	m_data[IO_READ]->Clear();
	WSABUF buf;
	buf.buf = m_data[IO_READ]->GetBuffer().GetData();
	buf.len = m_data[IO_READ]->GetBuffer().capacity();

	Recv(buf);
}

