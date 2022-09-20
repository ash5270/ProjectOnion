#pragma once
#include "SocketCommon.h"
#include <mstcpip.h>

namespace onion::socket
{
	class Session
	{
	protected:
		SOCKET m_socket;
		SOCKADDR_IN m_addrInfo;

	public:
		Session(const SOCKET& socket);
		virtual ~Session();
		
		virtual bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo);
		virtual void OnSend(size_t transferSize);
		virtual void RecvReady();
		virtual void OnRecv(size_t transferSize);
		virtual void OnClose();
	};
}


