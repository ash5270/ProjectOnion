
#include "IOCPData.h"

OnionSocket::IOCPData::IOCPData()
{
	m_type = IO_ERROR;
	ZeroMemory(&m_overlapped, sizeof(m_overlapped));

	this->Clear();
}

void OnionSocket::IOCPData::Clear()
{
	ZeroMemory(m_buffer, BUF_MAX_SIZE);
	m_totalBytes = 0;
	m_currentBytes = 0;
}

SOCKET OnionSocket::IOCPData::GetSocket() 
{
	//tcp_keepalive c;
	return m_socket;
}

void OnionSocket::IOCPData::SetSocket(const SOCKET& socket)
{
	m_socket = socket;
}

WSABUF* OnionSocket::IOCPData::GetWSABuf()
{
	return &m_wsabuf;
}

OVERLAPPED* OnionSocket::IOCPData::GetOverlapped()
{
	return &m_overlapped;
}

char* OnionSocket::IOCPData::GetData()
{
	return m_buffer;
}

OnionSocket::IO_TYPE OnionSocket::IOCPData::GetIOType() 
{
	return m_type;
}

void OnionSocket::IOCPData::SetIOType(IO_TYPE type)
{
	m_type = type;
}
