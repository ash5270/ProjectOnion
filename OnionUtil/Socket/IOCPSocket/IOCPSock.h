#pragma once
#include "../SocketCommon.h"
#include "../IOCPSocket/IOCPData.h"

#include <thread>

namespace onion::socket
{
	class IOCPSock
	{
	public:
		IOCPSock();
		virtual ~IOCPSock();

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
	private:
		bool m_bWorkerThread;
	};
}


