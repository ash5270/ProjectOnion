#pragma once
#include "SocketCommon.h"
#include "../Packet/Packet.h"
#include <mstcpip.h>

class PacketProcessSystem;

namespace onion::socket
{
	
	class Session
	{
	protected:
		SOCKET m_socket;
		SOCKADDR_IN m_addrInfo;
		PacketProcessSystem* m_packet_process_system;
	public:
		std::wstring userId;
		std::size_t userHash;
		const SOCKET& GetSocket() const
		{
			return m_socket;
		}

		void SetPacketProcessSystem(PacketProcessSystem* packet_process)
		{
			m_packet_process_system = packet_process;
		}
	public:
		Session(const SOCKET& socket);
		virtual ~Session();
		
		virtual bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo);
		virtual void OnSend(size_t transferSize);
		virtual void RecvReady();
		virtual void OnRecv(size_t transferSize);
		virtual void OnClose();
		virtual void SendPacket(Packet* packet);


	};
}


