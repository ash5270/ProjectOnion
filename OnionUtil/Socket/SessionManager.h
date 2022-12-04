#pragma once
#include "Session.h"
#include"../Util/SingleTon.h"
#include"../System/Buffer.h"

#include <list>

namespace onion::socket
{
	class SessionManager
	{
	
	public:
		SessionManager();
		virtual  ~SessionManager();

		void RegisterSession(Session* session);
		void UnregisterSession(Session* session);

		virtual std::list<Session*>* GetUserSessionList() = 0;
		
	};
}



