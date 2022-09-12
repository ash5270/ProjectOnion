#include "IOCPSock.h"
#include "IOCPSession.h"
#include <process.h>

unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	OnionSocket::IOCPSock* pOverlappedEvent = static_cast<OnionSocket::IOCPSock*>(p);
	pOverlappedEvent->WorkingThread();
	return 0;
}

OnionSocket::IOCPSock::IOCPSock()
{
	m_bWorkerThread = true;
}

OnionSocket::IOCPSock::~IOCPSock()
{
	//WSA 종료
	WSACleanup();
	if (m_pIocpsockinfo != nullptr)
	{
		delete[] m_pIocpsockinfo;
		m_pIocpsockinfo = nullptr;
	}

	if (m_pWorkerHandle != nullptr)
	{
		delete[] m_pWorkerHandle;
		m_pWorkerHandle = nullptr;
	}
}



bool OnionSocket::IOCPSock::CreateWorkerThread(size_t num_thread)
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
			printf_s("[error] worker Thread create Failed\n");
			return false;
		}
		ResumeThread(m_pWorkerHandle[i]);
	}
	printf_s("[info] worker thread  count : [%d]\n",num_thread+1);
	return true;
}

void OnionSocket::IOCPSock::WorkingThread()
{
	//function call success
	bool bResult;
	int nResult;

	//overlapped I/O send,recv data size
	DWORD recvBytes;
	DWORD sendBytes;

	//completion key pointer value
	IOCPSession* pCompletionkey;

	// I/O 작업을 위해 요청한 overlapped 구조체를 받을 포인터
	IOCPData* pSocketinfo;
	DWORD dwFlags = 0;


	while (m_bWorkerThread)
	{
		//이 함수로 인해 쓰레들은 waitingThread Queue에 대기상태로 들어가게 됨
		//완료된 overlapped I/O 작업이 발생하면 IOCPSock queue에서 완료된 작업을 가져와 처리함
		bResult = GetQueuedCompletionStatus(m_hIOCP, &recvBytes, (PULONG_PTR)&pCompletionkey, (LPOVERLAPPED*)&pSocketinfo, INFINITE);
		
		

		if (!bResult && recvBytes == 0)
		{
			printf_s("[info] socket(%I64d) connect close\n", pSocketinfo->GetSocket());
			closesocket(pSocketinfo->GetSocket());
			free(pSocketinfo);
			continue;
		}

		if (recvBytes == 0)
		{
			printf_s("[error] recvBytes size 0\n");
			closesocket(pSocketinfo->GetSocket());
			free(pSocketinfo);
			continue;
		}

		//pSocketinfo->GetWSABuf().len = recvBytes;

		switch (pSocketinfo->GetIOType())
		{
		case IO_READ:
			printf_s("[info] recv Success\n");
			break;

		case IO_WRITE:
			printf_s("[info] send Success\n");
			break;
		}

		pSocketinfo->Clear();

		
		//else
		//{
		//	printf_s("[info] message recv - bytes : [%d] Msg : [%s] \n", pSocketinfo->GetWSABuf()->len, pSocketinfo->GetWSABuf()->buf);
		//	//client recv buffer echo
		//	/*nResult = WSASend(pSocketinfo->GetSocket(), pSocketinfo->GetWSABuf(), 1, &sendBytes, dwFlags, NULL, NULL);
		//	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		//	{
		//		printf_s("[error] WSASend falied : %d \n", WSAGetLastError());
		//	}

		//	printf_s("[info] message send - Bytes : [%d], msg : [%s]\n", pSocketinfo->GetWSABuf()->len, pSocketinfo->GetWSABuf()->buf);*/

		//	//data reset
		//	//ZeroMemory(&(pSocketinfo->Overlapped()), sizeof(OVERLAPPED));
		//	/*pSocketinfo->GetWSABuf().len = BUF_SIZE;
		//	pSocketinfo->data_buf.buf = pSocketinfo->buffer;
		//	ZeroMemory(pSocketinfo->buffer, BUF_SIZE);
		//	pSocketinfo->recv_bytes = 0;
		//	pSocketinfo->send_bytes = 0;*/
		//	pSocketinfo->Clear();

		//	dwFlags = 0;
		//}
	}
}

void OnionSocket::IOCPSock::CloseSocket()
{
	WSACleanup();
}

bool OnionSocket::IOCPSock::WSAInit()
{
	int nResult = 0;
	WSAData wsaData;
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nResult!=0)
	{
		printf("[error] wsa init error\n");
		return false;
	}

	return true;
}
