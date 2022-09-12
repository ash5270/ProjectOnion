#pragma once
#include "SocketCommon.h"
#include <memory>
#include <mstcpip.h>

namespace onion::socket
{
	class Session
	//: public std::enable_shared_from_this<Session>
	{
	protected:
		SOCKET m_socket;
		SOCKADDR_IN m_addrInfo;

	public:
		virtual bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo);
		virtual void OnSend(size_t transferSize);
		virtual void RecvStandBy();
		virtual void OnRecv(size_t transferSize);
		virtual void OnClose();
	};
}


