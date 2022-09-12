#include "IOCPClient.h"

OnionSocket::IOCPClient::IOCPClient(std::string ip, int port) : m_ip(ip),m_port(port),m_isConnect(false)
{
	m_pIocpsockinfo = new IOCPData();
	m_session = new IOCPSession();
}

OnionSocket::IOCPClient::~IOCPClient()
{
}

bool OnionSocket::IOCPClient::InitializeClient()
{
	if (!WSAInit())
		return false;
	int nResult = 0;
	m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(m_socket==INVALID_SOCKET)
	{
		printf("[error] socket create error msg : [%s] \n", WSAGetLastError());
		return false;
	}
	

	//server info
	ZeroMemory(&m_serverAddr, sizeof(SOCKADDR_IN));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(m_port);
	nResult = inet_pton(AF_INET, m_ip.c_str(), &m_serverAddr.sin_addr.S_un.S_addr);
	 
	if(nResult==0)
	{
		printf("[error] inet_pton error\n");
		return false;
	}

	return true;
}

void OnionSocket::IOCPClient::StartClient()
{
	int nResult = 0;
	DWORD recvBytes;
	DWORD flags;

	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if(!CreateWorkerThread(1))
	{
		return; 
	}

	printf("[info] start client\n");

	nResult = connect(m_socket, reinterpret_cast<SOCKADDR*>(&m_serverAddr), sizeof(SOCKADDR_IN));
	if(nResult==SOCKET_ERROR)
	{
		printf("[error] connect failed\n");
		closesocket(m_socket);
		WSACleanup();
		return;
	}

	m_isConnect = true;

	flags = 0;

	m_hIOCP = CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_socket), m_hIOCP,
		(ULONG_PTR)&(m_session), NULL);


	DWORD get_size;

	/*ZeroMemory(m_pIocpsockinfo->GetOverlapped(), sizeof(OVERLAPPED));*/

	//m_pIocpsockinfo->GetWSABuf()->buf = sendBuf;
	//m_pIocpsockinfo->GetWSABuf()->len = 1024;
	//m_pIocpsockinfo->SetIOType(IO_WRITE);

	m_session->m_data[IO_WRITE]->GetWSABuf()->buf = sendBuf;
	m_session->m_data[IO_WRITE]->GetWSABuf()->len = 1024;

	
	nResult = WSASend(m_socket, m_session->m_data[IO_WRITE]->GetWSABuf(), 1, &get_size, 0, m_session->m_data[IO_WRITE]->GetOverlapped(), NULL);
	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf_s("[error] WSASend falied : %d \n", WSAGetLastError());
	}

	//nResult = WSARecv(m_pIocpsockinfo->GetSocket(), m_pIocpsockinfo->GetWSABuf(), 1, &recvBytes, &flags,
	//	m_pIocpsockinfo->GetOverlapped(), NULL);
	//if(nResult==SOCKET_ERROR&& WSAGetLastError()==WSA_IO_PENDING)
	//{
	//	printf_s("[error] IO pending failed : %d \n", WSAGetLastError());
	//}
}

void OnionSocket::IOCPClient::StopClient()
{
	WSACleanup();
}
