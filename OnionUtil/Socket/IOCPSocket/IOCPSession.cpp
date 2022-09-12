#include "IOCPSession.h"


OnionSocket::IOCPSession::IOCPSession()
{
	m_data[0] = new IOCPData();
	m_data[1] = new IOCPData();

	m_data[IO_READ]->SetIOType(IO_READ);
	m_data[IO_WRITE]->SetIOType(IO_WRITE);
}

OnionSocket::IOCPSession::~IOCPSession()
{

}


bool OnionSocket::IOCPSession::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	//return Session::OnAccept(socket, addrInfo);
	return false;
}

void OnionSocket::IOCPSession::Send(size_t transferSize)
{
	//Session::Send(transferSize);
}

void OnionSocket::IOCPSession::RecvStandBy()
{
	//Session::RecvStandBy();
}

void OnionSocket::IOCPSession::OnRecv()
{
	//Session::OnRecv();
}

void OnionSocket::IOCPSession::OnClose()
{
	//Session::OnClose();
}

