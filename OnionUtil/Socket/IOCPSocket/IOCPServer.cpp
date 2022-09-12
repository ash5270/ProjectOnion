#include "IOCPServer.h"

OnionSocket::IOCPServer::IOCPServer(int port) :IOCPSock()
{
	m_bAccept = true;
	m_port = port;
}

OnionSocket::IOCPServer::~IOCPServer()
{
	
}

bool OnionSocket::IOCPServer::InitializeServer()
{
	if (!WSAInit())
		return false;
	int nResult = 0;

	//listenSocket
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(m_listenSocket==INVALID_SOCKET)
	{
		printf_s("[error] listen socket create msg : [%s]\n", WSAGetLastError());
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
		printf_s("[error] bind failed\n");
		closesocket(m_listenSocket);
		WSACleanup();
		return false;
	}

	nResult = listen(m_listenSocket, 5);
	if (nResult != 0)
	{
		printf_s("[error] listen error\n");
		closesocket(m_listenSocket);
		WSACleanup();
		return false;
	}

	return true;
}

void OnionSocket::IOCPServer::StartServer()
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
	printf_s("[info] cpu count : %d \n", system_info.dwNumberOfProcessors);

	int nThreadCount = system_info.dwNumberOfProcessors * 2;


	//Worker Thread
	if (!CreateWorkerThread(nThreadCount-10))
	{
		return;
	}

	printf_s("[info] Server Start\n");


	m_thAccept = ::std::thread([&, this]()
		{
			while (m_bAccept)
			{
				//client connect
				clientSocket = WSAAccept(m_listenSocket, (sockaddr*)&clientAddr, &addrLen, NULL, NULL);
				if (clientSocket == INVALID_SOCKET)
				{
					printf_s("[error] Accept failed \n");
					return;
				}//client
				else
				{
					printf_s("[info] connect client \n ");
				}

				m_pIocpsockinfo = new IOCPData();
				m_pIocpsockinfo->SetIOType(IO_READ);
				m_pIocpsockinfo->SetSocket(clientSocket);
				m_pIocpsockinfo->GetWSABuf()->len = BUF_SIZE;
				m_pIocpsockinfo->GetWSABuf()->buf = m_pIocpsockinfo->GetData();

				flags = 0;

				m_hIOCP = CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), m_hIOCP, 
					reinterpret_cast<DWORD>(m_pIocpsockinfo), 0);

				//socket
				nResult = WSARecv(m_pIocpsockinfo->GetSocket(), m_pIocpsockinfo->GetWSABuf(), 1, &recvBytes, &flags, m_pIocpsockinfo->GetOverlapped(), NULL);
				if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
				{
					printf_s("[error] IO pending failed : %d\n", WSAGetLastError());
					return;
				}
			}
		});
}

void OnionSocket::IOCPServer::StopServer()
{
	m_thAccept.join();

	WSACleanup();
}
