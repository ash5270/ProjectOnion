#include "IOCPSessionManager.h"
#include "IOCPSession.h"
onion::socket::IOCPSessionManager::IOCPSessionManager()
{
}

onion::socket::IOCPSessionManager::~IOCPSessionManager()
{
}

void socket::IOCPSessionManager::RegisterSession(Session* session)
{
	if (session == nullptr)
		return;
	m_sessionList.push_back(session);
}

void socket::IOCPSessionManager::UnregisterSession(Session* session)
{
	if (session == nullptr)
		return;
	m_sessionList.remove(session);
}

bool onion::socket::IOCPSessionManager::InitSessionPool()
{
	return false;
}

void onion::socket::IOCPSessionManager::ReturnSession(IOCPSession* session)
{
}

std::list<socket::Session*>* socket::IOCPSessionManager::GetUserSessionList()
{
	return &m_sessionList;
}
