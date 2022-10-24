#include "IOCPSock.h"
#include "IOCPSession.h"
#include <process.h>


unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	onion::socket::IOCPSock* pOverlappedEvent = static_cast<onion::socket::IOCPSock*>(p);
	pOverlappedEvent->WorkingThread();
	return 0;
}

onion::socket::IOCPSock::IOCPSock()
{
	m_bWorkerThread = true;
}

onion::socket::IOCPSock::~IOCPSock()
{
	//WSA ����
	WSACleanup();

	if (m_pWorkerHandle != nullptr)
	{
		delete[] m_pWorkerHandle;
		m_pWorkerHandle = nullptr;
	}
}

bool onion::socket::IOCPSock::CreateWorkerThread(size_t num_thread)
{
	UINT32 threadId;
	//thread handler
	m_pWorkerHandle = new HANDLE[num_thread];
	//thread create
	for (int i = 0; i < num_thread; i++)
	{
		m_pWorkerHandle[i] = (HANDLE*)_beginthreadex(NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &threadId);
		if (m_pWorkerHandle[i] == NULL)
		{
			PO_LOG(LOG_ERROR, L"worker Thread create Failed\n");
			return false;
		}
		ResumeThread(m_pWorkerHandle[i]);
	}
	PO_LOG(LOG_INFO, L"worker thread  count : [%d]\n", num_thread);
	return true;
}

void onion::socket::IOCPSock::WorkingThread()
{
	//function call success
	bool bResult;

	//overlapped I/O send,recv data size
	DWORD recvBytes;
	DWORD sendBytes;

	//completion key pointer value
	IOCPSession* pSession;

	// I/O �۾��� ���� ��û�� overlapped ����ü�� ���� ������
	IOCPData* pSocketinfo;
	DWORD dwFlags = 0;

	while (m_bWorkerThread)
	{
		//�� �Լ��� ���� �������� waitingThread Queue�� �����·� ���� ��
		//�Ϸ�� overlapped I/O �۾��� �߻��ϸ� IOCPSock queue���� �Ϸ�� �۾��� ������ ó����
		bResult = GetQueuedCompletionStatus(m_hIOCP, &recvBytes, (PULONG_PTR)&pSession, (LPOVERLAPPED*)&pSocketinfo, INFINITE);

		if (!bResult && recvBytes == 0)
		{
			PO_LOG(LOG_INFO, L"socket(%I64d) connect close\n", pSocketinfo->GetSocket());
			pSession->OnClose();
			shutdown(pSocketinfo->GetSocket(), 2);
			closesocket(pSocketinfo->GetSocket());
			free(pSocketinfo);
			continue;
		}

		if (recvBytes == 0)
		{
			//PO_LOG(LOG_ERROR, L"recvBytes size 0\n");
			closesocket(pSocketinfo->GetSocket());
			free(pSocketinfo);
			continue;
		}

		switch (pSocketinfo->GetIOType())
		{
		case IO_READ:
			if(pSession!=nullptr)
			{
				//packet ������ ���� �ɰ�
				pSession->OnRecv(recvBytes);
			}
			//PO_LOG(LOG_INFO, L"recv success\n");
			pSession->RecvReady();

			break;

		case IO_WRITE:
			if (pSession != nullptr)
			{
				pSession->OnSend(recvBytes);
			}
			//PO_LOG(LOG_INFO, L"send success\n");
			break;
		}
	}
}

void onion::socket::IOCPSock::CloseSocket()
{
	WSACleanup();
}

bool onion::socket::IOCPSock::WSAInit()
{
	int nResult = 0;
	WSAData wsaData;
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nResult!=0)
	{
		PO_LOG(LOG_ERROR, L"wsa init error\n");
		return false;
	}

	return true;
}
