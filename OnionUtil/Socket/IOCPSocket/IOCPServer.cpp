#include "IOCPServer.h"
#include "IOCPSession.h"
#include"../../System/BufferPool.h"

onion::socket::IOCPServer::IOCPServer(int port) :IOCPSock()
{
	m_bAccept = true;
	m_port = port;
}

onion::socket::IOCPServer::~IOCPServer()
{
	
}

bool onion::socket::IOCPServer::InitializeServer()
{
	//log system 시작
	onion::system::LogSystem::getInstance().Start();
	onion::system::BufferPool::getInstance().Init(SERVER_BUFFER_POOL_SIZE);
	if (!WSAInit())
		return false;
	int nResult = 0;

	//listenSocket
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(m_listenSocket==INVALID_SOCKET)
	{
		PO_LOG(LOG_ERROR, L"listen socket create msg : [%d]\n", WSAGetLastError());
		return false;
	}

	//server info setting
	m_serverAdddr.sin_family = AF_INET;
	m_serverAdddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_serverAdddr.sin_port = htons(m_port);

	//socket setting
	nResult = bind(m_listenSocket, reinterpret_cast<SOCKADDR*>(&m_serverAdddr), sizeof(SOCKADDR_IN));
	if(nResult==SOCKET_ERROR)
	{
		PO_LOG(LOG_ERROR, L"bind failed\n");
		closesocket(m_listenSocket);
		WSACleanup();
		return false;
	}

	

	return true;
}

void onion::socket::IOCPServer::StartServer()
{
	int nResult = 0;
	//client 정보
	SOCKADDR_IN clientAddr;

	ZeroMemory(&clientAddr, sizeof(clientAddr));

	int addrLen = sizeof(clientAddr);
	SOCKET clientSocket = 0;
	DWORD recvBytes;
	DWORD flags;

	//Completion Port 객체 생성
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	//system info get
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	//printf_s("[info] cpu count : %d \n", system_info.dwNumberOfProcessors);

	int nThreadCount = system_info.dwNumberOfProcessors * 2;

	//Worker Thread
	if (!CreateWorkerThread(nThreadCount-10))
	{
		return;
	}

	PO_LOG(LOG_INFO, L"Server Start\n");

	nResult = listen(m_listenSocket, 5);
	if (nResult != 0)
	{
		PO_LOG(LOG_ERROR, L"listen failed\n");
		closesocket(m_listenSocket);
		WSACleanup();
		return;
	}


	//accept 
	m_thAccept = ::std::thread([&, this]()
		{
			while (m_bAccept)
			{
				//client connect
				clientSocket = WSAAccept(m_listenSocket, (sockaddr*)&clientAddr, &addrLen, NULL, NULL);
				if (clientSocket == INVALID_SOCKET)
				{
					PO_LOG(LOG_ERROR, L" Accept failed\n");
					return;
				}
				//session 생성
				auto session = new IOCPSession(clientSocket);
				//session 매니저에 추가
				SessionManager::getInstance().RegisterSession(session);

				auto io_data = session->m_data[IO_READ];
				//recv 받을 준비 해두기
				session->RecvReady();
				flags = 0;
				m_hIOCP = CreateIoCompletionPort(reinterpret_cast<HANDLE>(io_data->GetSocket()), m_hIOCP, 
					(ULONG_PTR)&(*session), 0);

				//accept 알려줌
				session->OnAccept(clientSocket, clientAddr);
			}
		});
}

void onion::socket::IOCPServer::StopServer()
{
	m_bAccept = false;

	if(m_thAccept.joinable())
		m_thAccept.join();

	WSACleanup();
	//log system 종료
	onion::system::LogSystem::getInstance().Stop();
	onion::system::BufferPool::getInstance().Delete();
}
