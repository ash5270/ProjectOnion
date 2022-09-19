#include "SessionManager.h"

onion::socket::SessionManager::SessionManager()
{

}

onion::socket::SessionManager::~SessionManager()
{

}

void onion::socket::SessionManager::RegisterSession(Session* session)
{
	if(session==nullptr)
		return;
	m_sessions.push_back(session);
}

void onion::socket::SessionManager::UnregisterSession(Session* session)
{
	if(session==nullptr)
		return;
	m_sessions.remove(session);
}


