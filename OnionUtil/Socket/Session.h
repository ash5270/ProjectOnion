#pragma once
#include "SocketCommon.h"
#include <mstcpip.h>
namespace OnionSocket
{
	class Session
	{
	protected:
		SOCKET m_socket;
		SOCKADDR_IN m_addrInfo;

		virtual bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo);

		virtual void Send(size_t transferSize);
		virtual void RecvStandBy();
		virtual void OnRecv();

		virtual void OnClose();
	};
}


