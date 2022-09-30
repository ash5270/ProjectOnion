#include "RIOSession.h"

onion::socket::RIOSession::RIOSession(const SOCKET& socket) : Session(socket)
{
	server = nullptr;
}

onion::socket::RIOSession::~RIOSession()
{
	
}

bool onion::socket::RIOSession::Init()
{
	m_rio_buffer_ptr = reinterpret_cast<char*>(VirtualAllocEx(GetCurrentProcess(), 0,
		SESSION_BUFFER_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
	if(m_rio_buffer_ptr==nullptr)
	{
		PO_LOG(LOG_ERROR, L"VirtualAllocEx Error : [%d]\n", GetLastError());
		return false;
	}

	// m_buffer = new onion::system::Buffer()
	m_buffer = new system::Buffer(m_rio_buffer_ptr, SESSION_BUFFER_SIZE);
	m_rioBufferID = RIOSock::m_Rio_func_table.RIORegisterBuffer(m_rio_buffer_ptr, SESSION_BUFFER_SIZE);

	if(m_rioBufferID == RIO_INVALID_BUFFERID)
	{
		PO_LOG(LOG_ERROR, L"RIORegisterBuffer Error : [%d]\n", GetLastError());
		return false;
	}

	return true;

}

void onion::socket::RIOSession::AddRef()
{
	long ref = InterlockedIncrement(&m_refCount);
	if(ref<0)
	{
		PO_LOG(LOG_ERROR, L"session refcount add error\n");
		return;
	}
}

void onion::socket::RIOSession::ReleaseRef()
{
	long ref = InterlockedDecrement(&m_refCount);
	if (ref < 0)
	{
		PO_LOG(LOG_ERROR, L"session refcount release error\n");
		return;
	}

	if (ref == 0)
	{
		//여기서 세션 삭제
		server->GetSessionManger().ReturnSession(this);
	}
}

long onion::socket::RIOSession::GetRef()
{
	return m_refCount;
}

void onion::socket::RIOSession::SetSocket(const SOCKET& socket)
{
	m_socket = socket;
}

SOCKET& onion::socket::RIOSession::GetSocket() 
{
	return m_socket;
}

void onion::socket::RIOSession::SetThreadID(int threadID)
{
	m_threadID = threadID;
}

bool onion::socket::RIOSession::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	AddRef();
	m_socket = socket;
	u_long arg = 1;
	ioctlsocket(m_socket, FIONBIO, &arg);

	int opt = 1;
	setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));

	//create socket rq
	//send랑 recv 를 한 cq에 씀 두개로 만들어서 사용해도 됨
	m_requestQueue = RIOSock::m_Rio_func_table.RIOCreateRequestQueue(m_socket, MAX_RECV_RQ_SIZE_PER_SOCK, 1, MAX_SEND_RQ_SIZE_PER_SOCK, 1
		, server->GetCompletionQueue(m_threadID),server->GetCompletionQueue(m_threadID),NULL);
	if(m_requestQueue==RIO_INVALID_RQ)
	{
		PO_LOG(LOG_ERROR, L"RIO create request queue Error : [%d]\n", GetLastError());
		return false;
	}

	RecvReady();
	return true;
}

void onion::socket::RIOSession::OnSend(size_t transferSize)
{
	PO_LOG(LOG_INFO, L"send success [%d] \n", transferSize);
}

void onion::socket::RIOSession::RecvReady()
{
	RIOContext* context = new RIOContext(this, IO_READ, m_rioBufferID);
	context->BufferId = m_rioBufferID;
	//버퍼에서 남은 자리 
	context->Length = static_cast<ULONG>(m_buffer->capacity()-m_buffer->size());
	context->Offset = m_buffer->size();

	DWORD recvBytes = 0;
	DWORD flags = 0;

	if (!RIOSock::m_Rio_func_table.RIOReceive(m_requestQueue, (PRIO_BUF)context, 1, flags, context))
	{
		PO_LOG(LOG_ERROR, L"RIOReceive error : [%d]\n", GetLastError());
		return;
	}

	//Session::RecvReady();
}

void onion::socket::RIOSession::OnRecv(size_t transferSize)
{
	PO_LOG(LOG_DEBUG, L"recv success msg size : [%d]\n",transferSize);
	std::wstring x;
	m_buffer->AddOffset(transferSize);
	*m_buffer >> &x;

	PO_LOG(LOG_INFO, L"[%s]\n", x.c_str());

	RIOContext* context = new RIOContext(this, IO_WRITE, m_rioBufferID);
	context->BufferId = m_rioBufferID;
	//버퍼에서 보낼 길이
	context->Length = static_cast<ULONG>(m_buffer->size());
	//버퍼 오프셋
	context->Offset = m_buffer->size()-transferSize;

	DWORD sendBytes = 0;
	DWORD flags = 0;


	if (!RIOSock::m_Rio_func_table.RIOSend(m_requestQueue, (PRIO_BUF)context, 1, flags, context))
	{
		PO_LOG(LOG_ERROR, L"RIOSend Error : [%d]\n", GetLastError());
		return;
	}

	//m_buffer->Clear();
	//Session::OnRecv(transferSize);
}

void onion::socket::RIOSession::OnClose()
{
	//Session::OnClose();
}

void onion::socket::RIOSession::SendBuffer(system::Buffer* buffer)
{
	//*m_buffer << buffer;

	RIOContext* context = new RIOContext(this, IO_WRITE, m_rioBufferID);
	context->BufferId = m_rioBufferID;
	//
	context->Length = static_cast<ULONG>(m_buffer->capacity() - m_buffer->size());
	context->Offset = m_buffer->size();

	DWORD sendBytes = 0;
	DWORD flags = 0;


	if (!RIOSock::m_Rio_func_table.RIOSend(m_requestQueue, (PRIO_BUF)context, 1, flags, context))
	{
		PO_LOG(LOG_ERROR, L"RIOSend Error : [%d]\n", GetLastError());
		return;
	}
}
