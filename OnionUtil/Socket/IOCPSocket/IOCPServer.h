#pragma once
#include "IOCPSock.h"
#include "../SessionManager.h"
#include <thread>

namespace onion::socket
{
	class IOCPServer : IOCPSock
	{
	public:
		IOCPServer(int port);
		~IOCPServer();

		//소켓 등록 및 서버 정보 설정
		bool InitializeServer();
		//서버 시작
		void StartServer();
		//서버 닫기
		void StopServer();

	private:
		std::thread m_thAccept;
		SOCKET m_listenSocket;
		SOCKADDR_IN m_serverAdddr;
		
		bool m_bAccept;
		int m_port;

	};

}

