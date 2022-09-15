#include "Session.h"

onion::socket::Session::Session()
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

void onion::socket::Session::RecvStandBy()
{

}

void onion::socket::Session::OnRecv(size_t transferSize)
{
	
}


void onion::socket::Session::OnClose()
{

}
