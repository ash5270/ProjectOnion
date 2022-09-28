#include "RIOContext.h"

onion::socket::RIOContext::RIOContext(Session* session, IO_TYPE io_type, RIO_BUFFERID bufferId)
{
	m_session = session;
	m_type = io_type;
	m_bufferId = bufferId;
}

IO_TYPE onion::socket::RIOContext::GetIOType()
{
	return m_type;
}

void onion::socket::RIOContext::SetIOType(IO_TYPE type)
{
	m_type = type;
}

onion::socket::Session* onion::socket::RIOContext::GetSession() const
{
	return m_session;
}

void onion::socket::RIOContext::SetSession(Session* session)
{
	m_session = session;
}
