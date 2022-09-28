#pragma once

#include "../SocketCommon.h"
#include "RIOContext.h"
namespace onion::socket
{
	//이거는 좀 더 찾아봐야할듯
	enum RioConfig
	{
		SESSION_BUFFER_SIZE = 65536,

		MAX_THREAD = 4,
		MAX_RESULT = 256,
		MAX_SEND_RQ_SIZE_PER_SOCK = 32,
		MAX_RECV_RQ_SIZE_PER_SOCK = 32,
		MAX_CLIENT_PER_RIO_THREAD=300, //ex 2560
		MAX_CQ_SIZE_PER_RIO_THREAD = (MAX_SEND_RQ_SIZE_PER_SOCK + MAX_RECV_RQ_SIZE_PER_SOCK) * MAX_CLIENT_PER_RIO_THREAD,
	};


	class RIOSock
	{
		static RIO_CQ m_Rio_CompletionQueue[5];
		bool m_bWorkerThread;
	public:
		RIOSock();
		~RIOSock();

		static unsigned int WINAPI CallWorkerThread(LPVOID p);
		bool CreateWorkerThread(size_t num_thread);
		void WorkingThread(UINT32 threadID);
		void CloseSocket();

		static RIO_EXTENSION_FUNCTION_TABLE m_Rio_func_table;
		RIO_CQ& GetCompletionQueue(int threaId) { return m_Rio_CompletionQueue[threaId]; }
	protected:
		bool WSAInit();

		HANDLE* m_pWorkerHandle;
	};

}

