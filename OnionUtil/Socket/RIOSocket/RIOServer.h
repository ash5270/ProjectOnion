﻿#pragma once
#include "RIOSock.h"
#include"RIOSessionManager.h"
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
		RIOServer(int port);
		~RIOServer();
		//
		bool InitializeServer();

		void StartServer();

		void StopServer();

		RIOSessionManager& GetSessionManger();
	};

}

