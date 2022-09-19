#pragma once
#include "Session.h"
#include"../Util/SingleTon.h"
#include"../System/Buffer.h"

#include <list>

namespace onion::socket
{
	class SessionManager : public util::SingleTon<SessionManager>
	{
		std::list<Session*> m_sessions;
	public:
		SessionManager();
		~SessionManager();

		void RegisterSession(Session* session);
		void UnregisterSession(Session* session);

	};
}



