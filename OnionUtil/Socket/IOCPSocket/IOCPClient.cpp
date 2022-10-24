#include "IOCPClient.h"
#include "../../System/LogSystem.h"

onion::socket::IOCPClient::IOCPClient(std::string ip, int port) : m_ip(ip),m_port(port),m_isConnect(false)
{
	
}

onion::socket::IOCPClient::~IOCPClient()
{

}

bool onion::socket::IOCPClient::InitializeClient()
{
	if (!WSAInit())
		return false;
	int nResult = 0;
	m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(m_socket==INVALID_SOCKET)
	{
		PO_LOG(LOG_ERROR, L"socket create error msg : [%s] \n", WSAGetLastError());
		return false;
	}
	//server info
	ZeroMemory(&m_serverAddr, sizeof(SOCKADDR_IN));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(m_port);
	nResult = inet_pton(AF_INET, m_ip.c_str(), &m_serverAddr.sin_addr.S_un.S_addr);

	//연결이 된다음 session 생성
	m_session = new IOCPSession(m_socket);
	m_session->OnAccept(m_socket, m_serverAddr);

	if(nResult==0)
	{
		PO_LOG(LOG_ERROR, L"inet_pton error\n");
		return false;
	}

	return true;
}

void onion::socket::IOCPClient::StartClient()
{
	int nResult = 0;
	DWORD recvBytes = 0;
	DWORD flags = 0;

	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if(!CreateWorkerThread(1))
	{
		return; 
	}

	PO_LOG(LOG_INFO, L"start client\n");

	nResult = connect(m_socket, reinterpret_cast<SOCKADDR*>(&m_serverAddr), sizeof(SOCKADDR_IN));
	if(nResult==SOCKET_ERROR)
	{
		PO_LOG(LOG_ERROR, L"connect failed\n");
		closesocket(m_socket);
		WSACleanup();
		return;
	}

	m_isConnect = true;

	m_session->RecvReady();

	m_hIOCP = CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_socket), m_hIOCP,
		(ULONG_PTR)&(*m_session), 0);

}

void onion::socket::IOCPClient::StopClient()
{
	WSACleanup();
}

onion::socket::IOCPSession* onion::socket::IOCPClient::GetSession()
{
	return m_session;
}
