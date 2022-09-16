#include "IOCPSession.h"
#include "cstdio"

onion::socket::IOCPSession::IOCPSession()
{
	m_data[0] = new IOCPData();
	m_data[1] = new IOCPData();

	m_data[IO_READ]->SetIOType(IO_READ);
	m_data[IO_WRITE]->SetIOType(IO_WRITE);

	m_socket = 0;
}

onion::socket::IOCPSession::~IOCPSession()
{

}


bool onion::socket::IOCPSession::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	//return Session::OnAccept(socket, addrInfo);
	return false;
}

void onion::socket::IOCPSession::OnSend(size_t transferSize)
{
	printf_s("[info] send size : [%d]\n", transferSize);

	//Session::OnSend(transferSize);
}


void onion::socket::IOCPSession::OnRecv(size_t transferSize)
{
	printf_s("[info] recv size : [%d]\n", transferSize);
	printf_s("[info] recv msg : [%s]\n", m_data[IO_READ]->GetBuffer().GetData());


	//Session::OnRecv();
}

void onion::socket::IOCPSession::OnClose()
{
	//Session::OnClose();
}

void onion::socket::IOCPSession::Send(WSABUF buffer)
{
	int nResult = 0;
	DWORD sendBytes;
	nResult = WSASend(this->m_socket, &buffer, 1, &sendBytes, 0, m_data[IO_WRITE]->GetOverlapped(), NULL);
	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf_s("[error] WSASend falied : %d \n", WSAGetLastError());
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
		printf_s("[error] IO pending failed : %d\n", WSAGetLastError());
	}
}

void onion::socket::IOCPSession::SendBuffer(system::Buffer* buffer)
{
	if (m_data[IO_WRITE]->GetIOType() != IO_WRITE)
		return;

	WSABUF buf;
	buf.buf = buffer->GetData();
	buf.len = buffer->size();

	this->Send(buf);
}

void onion::socket::IOCPSession::RecvReady()
{
	m_data[IO_READ]->Clear();
	WSABUF buf;
	buf.buf = m_data[IO_READ]->GetBuffer().GetData();
	buf.len = m_data[IO_READ]->GetBuffer().capacity();

	Recv(buf);
}

