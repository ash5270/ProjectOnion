#include "RIOServer.h"

#include "RIOSession.h"

onion::socket::RIOServer::RIOServer(int port)
{
	m_bAccept = true;
	m_port = port;
}

onion::socket::RIOServer::~RIOServer()
{
}

bool onion::socket::RIOServer::InitializeServer()
{
	onion::system::LogSystem::getInstance().Start();

	if (!WSAInit())
		return false;
	int nResult = 0;

	//
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_REGISTERED_IO);
	if(m_listenSocket==INVALID_SOCKET)
	{
		PO_LOG(LOG_ERROR, L"listen socket create msg : [%d]\n", WSAGetLastError());
		return false;
	}

	int option = 1;
	setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option));

	//server inf setting
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_serverAddr.sin_port = htons(m_port);

	//bind

	nResult = bind(m_listenSocket, reinterpret_cast<SOCKADDR*>(&m_serverAddr), sizeof(SOCKADDR_IN));
	if (nResult == SOCKET_ERROR)
	{
		PO_LOG(LOG_ERROR, L"bind failed\n");
		closesocket(m_listenSocket);
		WSACleanup();
		return false;
	}

	GUID functionTableID = WSAID_MULTIPLE_RIO;
	DWORD dwBytes = 0;

	if(WSAIoctl(m_listenSocket, SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER,&functionTableID,
		sizeof(GUID),(void**)&m_Rio_func_table,sizeof(m_Rio_func_table),&dwBytes,NULL,NULL))
	{
		return false;
	}

	return true;

}

void onion::socket::RIOServer::StartServer()
{
	m_session_manager.InitSessionPool();

	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);

	SOCKADDR_IN clientAddr;
	ZeroMemory(&clientAddr, sizeof(clientAddr));
	int addLen = sizeof(clientAddr);

	int nThreadCount = system_info.dwNumberOfProcessors * 2;

	//if(!CreateWorkerThread(nThreadCount-10))
	if(!CreateWorkerThread(MAX_THREAD))
	{
		return;
	}

		
	int nResult = 0;
	nResult = listen(m_listenSocket, 5);
	if (nResult == SOCKET_ERROR)
	{
		return;
	}

	m_thAccept = std::thread([&, this]
		{
			while (m_bAccept)
			{
				SOCKET client = accept(m_listenSocket, (sockaddr*)&clientAddr, &addLen);
				if(client ==INVALID_SOCKET)
				{
					PO_LOG(LOG_ERROR, L" Accept failed\n");
					return;
				}
				RIOSession* session = m_session_manager.IssueSession();
				session->server = this;
				session->OnAccept(client, clientAddr);
			}
		});
}

void onion::socket::RIOServer::StopServer()
{
}
