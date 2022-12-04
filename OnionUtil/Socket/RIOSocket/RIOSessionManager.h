#pragma once
#include <list>
#include "RIOSock.h"
#include "../../System/SpinLock.h"
#include "../SessionManager.h"
namespace onion::socket
{
	class RIOSession;
	class RIOSessionManager  :public SessionManager
	{
		uint64_t m_currentIssueCount;
		uint64_t m_currentReturnCount;

		std::list<Session*> m_freeSessionList[MAX_THREAD + 1];
		std::list<Session*> m_useSessionList;

		SpinLock m_lock;
	public:
		RIOSessionManager();
		~RIOSessionManager();

		bool InitSessionPool();

		RIOSession* IssueSession();

		void ReturnSession(RIOSession* session);

		std::list<Session*>* GetUserSessionList() override
		{
			return &m_useSessionList;
		}
	};
}

