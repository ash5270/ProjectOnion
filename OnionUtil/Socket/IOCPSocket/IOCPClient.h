﻿#pragma once
#include "IOCPSock.h"
#include "IOCPSession.h"
#include "../../System/BufferPool.h"
#include <string>


namespace onion::socket
{
	class IOCPClient :public IOCPSock
	{
	public:
		IOCPClient(std::string ip, int port);
		~IOCPClient();

		//소켓 등록 및 클라 정보 설정
		bool InitializeClient();
		//클라 시작
		void StartClient();
		//클라 닫기
		void StopClient();

		IOCPSession* GetSession();

	private:
		SOCKET m_socket;
		SOCKADDR_IN m_serverAddr;
		int m_port;
		std::string m_ip;
		bool m_isConnect;
		IOCPSession* m_session;
	};
}


