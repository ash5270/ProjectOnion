#pragma once
#include <list>
#include "RIOSock.h"
#include "../../System/SpinLock.h"

namespace onion::socket
{
	class RIOSession;
	class RIOSessionManager 
	{
		uint64_t m_currentIssueCount;
		uint64_t m_currentReturnCount;

		std::list<RIOSession*> m_freeSessionList[MAX_THREAD + 1];
		std::list<RIOSession*> m_useSessionList;

		SpinLock m_lock;
	public:
		RIOSessionManager();
		~RIOSessionManager();

		bool InitSessionPool();

		RIOSession* IssueSession();

		void ReturnSession(RIOSession* session);
	};
}

