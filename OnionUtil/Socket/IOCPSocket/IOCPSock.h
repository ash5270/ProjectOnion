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
	private:
		bool m_bWorkerThread;
	};
}


