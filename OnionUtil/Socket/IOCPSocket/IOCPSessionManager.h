#pragma once
#include <list>
#include "IOCPSock.h"
#include "../../System/SpinLock.h"
#include"../SessionManager.h"

namespace onion::socket {
	class Session;
	class IOCPSession;
	class IOCPSessionManager :public SessionManager
	{
	public:
		std::list<Session*> m_sessionList;

		SpinLock m_lock;
	public:
		IOCPSessionManager();
		~IOCPSessionManager();

		void RegisterSession(Session* session);
		void UnregisterSession(Session* session);

		bool InitSessionPool();

		void ReturnSession(IOCPSession* session);

		std::list<Session*>* GetUserSessionList() override;
		
	};
}

