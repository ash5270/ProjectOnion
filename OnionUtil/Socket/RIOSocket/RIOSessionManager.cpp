#include "RIOSessionManager.h"
#include "RIOSession.h"
onion::socket::RIOSessionManager::RIOSessionManager() :m_currentIssueCount(0),m_currentReturnCount(0)
{
}

onion::socket::RIOSessionManager::~RIOSessionManager()
{
	for(auto it : m_useSessionList)
	{
		delete it;
	}

	for(int i=0; i<= MAX_THREAD;i++)
	{
		for(auto it : m_freeSessionList[i])
		{
			delete it;
		}
	}
}

bool onion::socket::RIOSessionManager::InitSessionPool()
{
	for(int i=0;i<=MAX_THREAD; i++)
	{
		for(int j=0; j<MAX_CLIENT_PER_RIO_THREAD; j++)
		{
			RIOSession* session = new RIOSession(NULL);
			session->SetThreadID(i);
			if(!session->Init())
			{
				return false;
			}

			m_freeSessionList[i].push_back(session);
		}
	}
	return true;
}

onion::socket::RIOSession* onion::socket::RIOSessionManager::IssueSession()
{
	SpinLockGuard lock(m_lock);
	uint64_t threadID = (m_currentIssueCount++ % MAX_THREAD) + 1;
	if(threadID<0)
	{
		PO_LOG(LOG_ERROR, L"Thread ID Error");
		return nullptr;
	}

	RIOSession* session = m_freeSessionList[threadID].back();
	m_freeSessionList[threadID].pop_back();

	session->AddRef();

	m_useSessionList.push_back(session);
	return session;
}

void onion::socket::RIOSessionManager::ReturnSession(RIOSession* session)
{
	SpinLockGuard lock(m_lock);
	//여기서 소켓 connect 유뮤 , ref count 0 일때 확인
	if(session->GetSocket()!=NULL&& session->GetRef()!=0)
	{
		PO_LOG(LOG_ERROR, L"Session using");
		return;
	}
	uint64_t threadID = (m_currentReturnCount++ % MAX_THREAD) + 1;
	if (threadID < 0)
	{
		PO_LOG(LOG_ERROR, L"Thread ID Error");
		return;
	}

	m_useSessionList.remove(session);
	m_freeSessionList[threadID].push_back(session);

}
