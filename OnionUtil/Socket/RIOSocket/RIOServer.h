#pragma once
#include "RIOSock.h"
#include"RIOSessionManager.h"
#include"../../Packet/ProcessCommon.h"
#include <thread>

namespace onion::socket
{
	class RIOServer : public RIOSock
	{
		RIOSessionManager m_session_manager;
		std::thread m_thAccept;
		SOCKET m_listenSocket;
		SOCKADDR_IN m_serverAddr;

		bool m_bAccept;
		int m_port;
		
	public:
		PacketProcessSystem* packet_process;
		RIOServer(int port);
		~RIOServer();
		//
		bool InitializeServer();

		void StartServer();

		void StopServer();

		void Update();

		RIOSessionManager& GetSessionManger();


		//Login
		packet::process::LoginProcess* login_process;
		packet::process::CharacterProcess* character_process;
		packet::process::ChannelProcess* channel_process;
		
		
		
	};

}

