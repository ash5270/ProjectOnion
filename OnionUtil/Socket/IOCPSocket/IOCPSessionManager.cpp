#include "IOCPSessionManager.h"
#include "IOCPSession.h"
onion::socket::IOCPSessionManager::IOCPSessionManager()
{
}

onion::socket::IOCPSessionManager::~IOCPSessionManager()
{
}

bool onion::socket::IOCPSessionManager::InitSessionPool()
{
	return false;
}

void onion::socket::IOCPSessionManager::ReturnSession(IOCPSession* session)
{
}

std::list<socket::IOCPSession*>& socket::IOCPSessionManager::GetSessionList()
{
	return m_sessionList;
}
