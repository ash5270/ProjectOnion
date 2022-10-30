#pragma once
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")

#define SERVER_PORT 3000
#include "../Util/Common.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <atomic>

#include "../System/LogSystem.h"

typedef enum
{
	IO_READ = 0,
	IO_WRITE,
	IO_ACCEPT,
	IO_ERROR
}IO_TYPE;

