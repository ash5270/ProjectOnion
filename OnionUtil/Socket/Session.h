#pragma once
#include "SocketCommon.h"
#include <memory>
#include <mstcpip.h>

namespace onion::socket
{
	class Session
	{
	protected:
		SOCKET m_socket;
		SOCKADDR_IN m_addrInfo;

	public:
		Session();
		virtual ~Session();
		
		virtual bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo);
		virtual void OnSend(size_t transferSize);
		virtual void RecvStandBy();
		virtual void OnRecv(size_t transferSize);
		virtual void OnClose();
	};
}


