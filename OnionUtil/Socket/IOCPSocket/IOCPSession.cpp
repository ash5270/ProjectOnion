#include "IOCPSession.h"
#include "cstdio"
#include "../../Packet/PacketFactory.h"

onion::socket::IOCPSession::IOCPSession(const SOCKET& socket) : Session(socket)
{
	m_data[0] = new IOCPData();
	m_data[1] = new IOCPData();


	m_socket = socket;

	//IOCP Data에 socket 데이터 등록
	m_data[IO_READ]->SetSocket(m_socket);
	m_data[IO_WRITE]->SetSocket(m_socket);
	//IOCP Data 타입 지정
	m_data[IO_READ]->SetIOType(IO_READ);
	m_data[IO_WRITE]->SetIOType(IO_WRITE);
}

onion::socket::IOCPSession::~IOCPSession()
{
	if(m_data[IO_READ]!=nullptr)
	{
		delete m_data[IO_READ];
		m_data[IO_READ] = nullptr;
	}

	if (m_data[IO_WRITE] != nullptr)
	{
		delete m_data[IO_WRITE];
		m_data[IO_WRITE] = nullptr;
	}
}


bool onion::socket::IOCPSession::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	int opt = 1;
	setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));


	//수신 버퍼 비우기
	char tmp_char;
	u_long tmp_long = 0;
	long i;
	if(ioctlsocket(m_socket,FIONREAD,&tmp_long)!=SOCKET_ERROR)
	{
		for(int i=0; i< tmp_long;i++)
		{
			recv(m_socket, &tmp_char, sizeof(char), 0);
		}
	}

	PO_LOG(LOG_INFO, L"Accpet client\n");
	m_isConnect = true;
	m_is_sending.store(false);
	return true;
}

void onion::socket::IOCPSession::OnSend(size_t transferSize)
{
	
	//PO_LOG(LOG_INFO, L"[info] send size : [%d]\n", transferSize);
	//여기서 다시 남은 sendqueue에 있는 데이터 보내기
	//PO_LOG(LOG_INFO, L"thread id : [%d]\n", std::this_thread::get_id());
	//보내서 꽉차면 앞에다 넣기
	if(m_buf_queue.empty())
	{
		bool sending = true;
		m_is_sending.compare_exchange_strong(sending, false);
		return;
	}

	if (!m_isConnect)
		return;

	//lock 락 걸기 
	m_data[IO_WRITE]->GetBuffer().Clear();

	int size = 0;
	for(int i=0; i < m_buf_queue.size(); i++)
	{
		//front front 분리해서 만들어야할듯
		Buffer* send_buf = m_buf_queue.front();
		if (send_buf == nullptr)
		{
			//PO_LOG(LOG_INFO, L"[error] send_buf\n");
			return;
		}
		if(!(m_data[IO_WRITE]->GetBuffer()<<*send_buf))
		{
			//m_data[IO_WRITE]->GetBuffer().HeadClear();
			break;
		}
		m_buf_queue.pop();
		//PO_LOG(LOG_INFO, L"%d\n", send_buf->write_size); 
	}
	//printf("sned\n");
	if (m_data[IO_WRITE]->GetBuffer().size() > 0)
	{
		WSABUF Wsabuf;
		Wsabuf.buf = m_data[IO_WRITE]->GetBuffer().GetData();
		Wsabuf.len = m_data[IO_WRITE]->GetBuffer().offset() ;
		this->Send(Wsabuf);
	}

	//PO_LOG(LOG_ERROR, L" queue size %d \n", m_buf_queue.size());

	if(m_buf_queue.empty())
	{
		bool sending = true;
		m_is_sending.compare_exchange_strong(sending, false);
		return;
	}
	//buffer->
	
}


void onion::socket::IOCPSession::OnRecv(size_t transferSize)
{
	m_data[IO_READ]->GetBuffer().HeadCommit(transferSize);
	PO_LOG(LOG_INFO, L"[info] recv size : [%d]\n", transferSize);
	//Session::OnRecv();
}

void onion::socket::IOCPSession::OnClose()
{
	m_isConnect = false;
	PO_LOG(LOG_INFO, L"[info]  disconnect\n");
}

void onion::socket::IOCPSession::Send(WSABUF buffer)
{
	int nResult = 0;
	DWORD sendBytes;
	nResult = WSASend(this->m_socket, &buffer, 1, &sendBytes, 0, m_data[IO_WRITE]->GetOverlapped(), NULL);
	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		PO_LOG(LOG_ERROR, L"WSASend falied : %d\n", WSAGetLastError());
	}
}

void onion::socket::IOCPSession::Recv(WSABUF buffer)
{
	int nResult = 0;
	DWORD recvBytes = 0;
	DWORD flags = 0;
	nResult = WSARecv(this->m_socket, &buffer, 1, &recvBytes, &flags, m_data[IO_READ]->GetOverlapped(), NULL);
	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		PO_LOG(LOG_ERROR, L"IO pending failed : %d\n", WSAGetLastError());
	}
}

void onion::socket::IOCPSession::SendBuffer(system::Buffer* buffer)
{
	if (m_data[IO_WRITE]->GetIOType() != IO_WRITE)
		return;

	m_data[IO_WRITE]->Clear();
	WSABUF buf;
	buf.buf = buffer->GetData();
	buf.len = buffer->size();

	this->Send(buf);
}

void onion::socket::IOCPSession::SendPacket(Packet* packet)
{
	if (!m_isConnect)
		return;
	Buffer* buf = new Buffer(128);
	//PK_S_ANS_CHATTING* p = reinterpret_cast<PK_S_ANS_CHATTING*>(packet);
	auto header = packet->Serialize(*buf);
	header->size = buf->write_size;

	m_buf_queue.push_back(buf);
	bool sending = false;
	////보내고 있는 동안 여기 와서 에러 나는거 같은데??
	if(m_is_sending.compare_exchange_strong(sending, true))
	{
		m_data[IO_WRITE]->Clear();
		m_buf_queue.pop();
		if(!(m_data[IO_WRITE]->GetBuffer() << *buf))
		{
			printf("dd\n");
		}
		WSABUF Wsabuf;
		Wsabuf.buf = m_data[IO_WRITE]->GetBuffer().GetData();
		Wsabuf.len = m_data[IO_WRITE]->GetBuffer().offset()-m_data[IO_WRITE]->GetBuffer().tailOffset();
		this->Send(Wsabuf);
	}
}
	
void onion::socket::IOCPSession::RecvReady()
{
	//수정해야할곳 
	m_data[IO_READ]->Clear();
	WSABUF buf;
	buf.buf = m_data[IO_READ]->GetBuffer().GetData();
	buf.len = m_data[IO_READ]->GetBuffer().capacity();

	this->Recv(buf);
}

