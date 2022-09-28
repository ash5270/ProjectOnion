#pragma once
#include "../SocketCommon.h"
#include "../Session.h"
namespace onion::socket
{
	//iocp data 같은 느낌
	class RIOContext : public RIO_BUF
	{
		IO_TYPE m_type;
		Session* m_session;

	public:
		RIO_BUFFERID m_bufferId;
		RIOContext(Session* session, IO_TYPE io_type, RIO_BUFFERID bufferId);

		IO_TYPE GetIOType();
		void SetIOType(IO_TYPE type);
		Session* GetSession() const;
		void SetSession(Session* session);
	};

}
