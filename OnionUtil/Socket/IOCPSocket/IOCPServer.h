#pragma once
#include "IOCPSock.h"
#include "../SessionManager.h"
#include "../../System/BufferPool.h"
#include"../../Packet/ProcessCommon.h"
#include"IOCPSessionManager.h"
#include <thread>

namespace onion::socket
{
	class IOCPServer : public IOCPSock
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
		//서버 업데이트
		void Update();

	private:
		std::thread m_thAccept;
		SOCKET m_listenSocket;
		SOCKADDR_IN m_serverAdddr;
		
		bool m_bAccept;
		int m_port;

		IOCPSessionManager m_session_manager;

		Channel* m_channel;
		PacketProcessSystem* m_packet_process;
		//Login
		packet::process::LoginProcess* login_process;
		packet::process::CharacterProcess* character_process;
		packet::process::ChannelProcess* channel_process;
		packet::process::BasicPacketProcess* basic_process;
	};

}

