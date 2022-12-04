#include "IOCPDummyClient.h"

#include "IOCPSession.h"
#include"../../System/LogSystem.h"
#include "../../System/BufferPool.h"

onion::socket::IOCPDummyClient::IOCPDummyClient(std::string ip, int port) : m_ip(ip), m_port(port)
{

}

onion::socket::IOCPDummyClient::~IOCPDummyClient()
{
}

bool onion::socket::IOCPDummyClient::InitializeClient(int SessionCount)
{
	//system::BufferPool::getInstance().Init(CLIENT_BUFFER_POOL_SIZE);
	system::LogSystem::getInstance().Start();
	m_packetProcess = new PacketProcessSystem();
	m_packetProcess->Start();

	if (!WSAInit())
		return false;

	int nResult = 0;

	ZERO_MEMORY(&m_serverAddr, sizeof(SOCKADDR_IN));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(m_port);
	nResult = inet_pton(AF_INET, m_ip.c_str(), &m_serverAddr.sin_addr.S_un.S_addr);

	if (nResult == 0)
	{
		PO_LOG(LOG_ERROR, L"inet_pton error\n");
		return false;
	}

	for (int i = 0; i < SessionCount; i++)
	{
		SOCKET socekt = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		IOCPSession* session = new IOCPSession(socekt);
		m_sessions.GetUserSessionList()->push_back(session);
		session->SetPacketProcessSystem(m_packetProcess);
	}


	return true;
}

void onion::socket::IOCPDummyClient::StartDummyClient()
{
	int nResult = 0;

	m_hIOCP = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		0,
		0);

	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	if (!CreateWorkerThread(system_info.dwNumberOfProcessors/2))
	{
		return;
	}

	PO_LOG(LOG_INFO, L"start client...\n");

	for (auto session : *m_sessions.GetUserSessionList())
	{
		nResult = connect(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&m_serverAddr), sizeof(SOCKADDR_IN));
		if (nResult == SOCKET_ERROR)
		{
			break;
		}
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(session->GetSocket()), m_hIOCP,
			(ULONG_PTR) & (*session), 0);
		session->RecvReady();
		session->OnAccept(session->GetSocket(), m_serverAddr);
	}

	if (nResult == SOCKET_ERROR)
	{
		for(auto session : *m_sessions.GetUserSessionList())
		{
			PO_LOG(LOG_ERROR, L"connect failed\n");
			closesocket(session->GetSocket());
			WSACleanup();
			return;
		}
	}
}

void onion::socket::IOCPDummyClient::StopClient()
{
	system::LogSystem::getInstance().Stop();
	//onion::system::BufferPool::getInstance().Delete();
	m_packetProcess->Stop();
	WSACleanup();
}

socket::IOCPSessionManager* socket::IOCPDummyClient::GetSessions()
{
	return &m_sessions;
}
