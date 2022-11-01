#include "RIOSock.h"
#include "RIOSession.h"

RIO_CQ onion::socket::RIOSock::m_Rio_CompletionQueue[onion::socket::MAX_THREAD] = {0};
RIO_EXTENSION_FUNCTION_TABLE onion::socket::RIOSock::m_Rio_func_table = { 0 };

onion::socket::RIOSock::RIOSock()
{

}

onion::socket::RIOSock::~RIOSock()
{
	WSACleanup();
}

//
unsigned int WINAPI onion::socket::RIOSock::CallWorkerThread(LPVOID p)
{
	auto threadID = reinterpret_cast<UINT32>(p);
	m_Rio_CompletionQueue[threadID] = 
		RIOSock::m_Rio_func_table.RIOCreateCompletionQueue(MAX_CQ_SIZE_PER_RIO_THREAD, 0);
	if(m_Rio_CompletionQueue[threadID] == RIO_INVALID_CQ)
	{
		return -1;
	}


	RIORESULT results[MAX_RESULT];

	while(true)
	{
		memset(results, 0, sizeof(results));

		ULONG num_results = RIOSock::m_Rio_func_table.RIODequeueCompletion(m_Rio_CompletionQueue[threadID], results, MAX_RESULT);

		if (0 == num_results)
		{
			Sleep(1);
			continue;
		}else if(RIO_CORRUPT_CQ==num_results)
		{
			PO_LOG(LOG_ERROR, L"RIO CORRUPT CQ\n");
			break;
		}

		for (ULONG i = 0; i < num_results; i++)
		{
			RIOContext* context = reinterpret_cast<RIOContext*>(results[i].RequestContext);
			RIOSession* session = reinterpret_cast<RIOSession*>(context->GetSession());
			ULONG transferred = results[i].BytesTransferred; 

			if(transferred==0)
			{
				PO_LOG( LOG_INFO, L" error = [%d] ,context type = [%d]\n",GetLastError(),context->GetIOType());
				closesocket(session->GetSocket());
				shutdown(session->GetSocket(), 2);
				session->OnClose();
				continue;
			}
			else if(context->GetIOType()==IO_READ)
			{
				session->OnRecv(transferred);
				session->RecvReady();
			}
			else if(context->GetIOType()==IO_WRITE)
			{
				session->OnSend(transferred);
			}else
			{
				PO_LOG(LOG_ERROR, L"Unkown I/O type : %d \n", context->GetIOType());
				break;
			}
			session->ReleaseRef();
		}
	}

	return 0;
}
//thread 생성
bool onion::socket::RIOSock::CreateWorkerThread(size_t num_thread)
{
	UINT32 threadId;
	//thread handler
	m_pWorkerHandle = new HANDLE[num_thread];
	for (int i = 0; i < num_thread; i++)
	{
		m_pWorkerHandle[i] = (HANDLE*)_beginthreadex(NULL, 0,  &RIOSock::CallWorkerThread, (LPVOID)(i+1), CREATE_SUSPENDED, &threadId);
		if (m_pWorkerHandle[i] == NULL)
		{
			PO_LOG(LOG_ERROR, L"worker Thread create Failed\n");
			return false;
		}
		ResumeThread(m_pWorkerHandle[i]);
	}
	PO_LOG(LOG_INFO, L"worker thread  count : [%d]\n", num_thread + 1);
	return true;
}

void onion::socket::RIOSock::WorkingThread(UINT32 threadID)
{
	
}

void onion::socket::RIOSock::CloseSocket()
{

}

bool onion::socket::RIOSock::WSAInit()
{
	int nReuslt = 0;
	WSADATA wsaData;

	nReuslt = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nReuslt!=0)
	{
		PO_LOG(LOG_ERROR, L"wsa init error\n");
		return false;
	}

	return true;
}
