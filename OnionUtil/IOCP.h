#pragma once
#include "Socket/SocketCommon.h"
#include "Socket/IOCP/IOCPData.h"
#include <thread>

namespace OnionSocket
{
	#define BUF_SIZE 1024

	class IOCP
	{
	public:
		IOCP();
		~IOCP();

		
		//작업 스레드 생성
		bool CreateWorkerThread(size_t num_thread);
		//작업 스레드 시작
		void WorkingThread();
		//소켓 닫기 
		void CloseSocket();
	
	protected:
		bool WSAInit();

		HANDLE* m_pWorkerHandle;
		HANDLE m_hIOCP;
		IOCPData* m_pIocpsockinfo;
	private:
		bool m_bWorkerThread;
	};
}


