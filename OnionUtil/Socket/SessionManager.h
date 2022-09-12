#pragma once
#include "Session.h"
#include"../Util/SingleTon.h"

#include <memory>
#include <list>

namespace onion::socket
{
	class SessionManager : public util::SingleTon<SessionManager>
	{
	public:
		SessionManager();
		~SessionManager();

		void CreateSession(Session* session);
		void DestorySession(Session* session);
	private:
		std::list<Session*> m_sessions;
	};
}



