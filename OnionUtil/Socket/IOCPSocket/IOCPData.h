#pragma once
#include "../SocketCommon.h"

namespace OnionSocket
{
	typedef enum
	{
		IO_READ =0,
		IO_WRITE ,
		IO_ACCEPT,
		IO_ERROR
	}IO_TYPE;

	class IOCPData
	{
		OVERLAPPED m_overlapped;
		IO_TYPE m_type;
		size_t m_totalBytes;
		size_t m_currentBytes;
		SOCKET m_socket;
		WSABUF m_wsabuf;

		char m_buffer[BUF_MAX_SIZE];
	public:
		IOCPData();
		void Clear();
		SOCKET GetSocket();
		void SetSocket(const SOCKET& socket);
		WSABUF* GetWSABuf();
		OVERLAPPED* GetOverlapped();
		char* GetData();
		IO_TYPE GetIOType();
		void SetIOType(IO_TYPE type);
	};
}