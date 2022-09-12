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

void onion::socket::IOCPSession::RecvStandBy()
{

	//Session::RecvStandBy();
}

void onion::socket::IOCPSession::OnRecv(size_t transferSize)
{
	printf_s("[info] recv size : [%d]\n", transferSize);
	printf_s("[info] recv msg : [%s]\n", m_data[IO_READ]->GetData());


	//Session::OnRecv();
}

void onion::socket::IOCPSession::OnClose()
{
	//Session::OnClose();
}

