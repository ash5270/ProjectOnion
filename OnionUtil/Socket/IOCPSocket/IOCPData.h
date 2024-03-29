﻿#pragma once
#include "../SocketCommon.h"
#include "../../System/Buffer.h"
#include "../../System/CircularBuffer.h"


namespace onion::socket
{
	class IOCPData
	{
		OVERLAPPED m_overlapped;
		IO_TYPE m_type;
		size_t m_totalBytes;
		size_t m_currentBytes;
		SOCKET m_socket;
		system::CircularBuffer m_buffer;
	public:
		IOCPData();
		void Clear();
		SOCKET GetSocket();
		void SetSocket(const SOCKET& socket);
		WSABUF GetWSABuf();
		OVERLAPPED* GetOverlapped();
		system::CircularBuffer& GetBuffer();
		IO_TYPE GetIOType();
		void SetIOType(IO_TYPE type);

		void SetData(system::CircularBuffer buffer);
	};
}