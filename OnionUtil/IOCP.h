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

		
		//�۾� ������ ����
		bool CreateWorkerThread(size_t num_thread);
		//�۾� ������ ����
		void WorkingThread();
		//���� �ݱ� 
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


