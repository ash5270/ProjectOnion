#include "IOCPData.h"

onion::socket::IOCPData::IOCPData()
{
	m_type = IO_ERROR;
	ZeroMemory(&m_overlapped, sizeof(m_overlapped));

	this->Clear();
}

void onion::socket::IOCPData::Clear()
{
	//PO_LOG(LOG_ERROR, L"초기화 iodate\n");
	m_buffer.Clear();
	m_totalBytes = 0;
	m_currentBytes = 0;
	GetBuffer().write_size = 0;
}

SOCKET onion::socket::IOCPData::GetSocket()
{
	//tcp_keepalive c;
	return m_socket;
}

void onion::socket::IOCPData::SetSocket(const SOCKET& socket)
{
	m_socket = socket;
}

WSABUF onion::socket::IOCPData::GetWSABuf()
{
	WSABUF buf;
	buf.len = 0;
	buf.buf = m_buffer.GetData() + m_buffer.size();
	return buf;
}

OVERLAPPED* onion::socket::IOCPData::GetOverlapped()
{
	return &m_overlapped;
}

onion::system::Buffer& onion::socket::IOCPData::GetBuffer()
{
	return m_buffer;
}

IO_TYPE onion::socket::IOCPData::GetIOType()
{
	return m_type;
}

void onion::socket::IOCPData::SetIOType(IO_TYPE type)
{
	m_type = type;
}
