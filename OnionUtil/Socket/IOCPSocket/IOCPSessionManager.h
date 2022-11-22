#pragma once
#include <list>
#include "IOCPSock.h"
#include "../../System/SpinLock.h"

namespace onion::socket {
	class IOCPSession;
	class IOCPSessionManager
	{
	public:
		std::list<IOCPSession*> m_sessionList;

		SpinLock m_lock;
	public:
		IOCPSessionManager();
		~IOCPSessionManager();

		bool InitSessionPool();

		void ReturnSession(IOCPSession* session);

		std::list<IOCPSession*>& GetSessionList();
		
	};
}

