#include "Session.h"

onion::socket::Session::Session(const SOCKET& socket) : m_socket(socket)
{

}

onion::socket::Session::~Session()
{

}

bool onion::socket::Session::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo) 
{
	return false;
}

void onion::socket::Session::OnSend(size_t transferSize)
{

}

void onion::socket::Session::RecvReady()
{

}

void onion::socket::Session::OnRecv(size_t transferSize)
{
	
}


void onion::socket::Session::OnClose()
{

}

void onion::socket::Session::SendPacket(Packet* packet)
{

}
