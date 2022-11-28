#include "BasicPacketProcess.h"

onion::packet::process::BasicPacketProcess::BasicPacketProcess(RIOSessionManager* manager)
{
	m_sessionManager = manager;
}

onion::packet::process::BasicPacketProcess::~BasicPacketProcess()
{
}

void onion::packet::process::BasicPacketProcess::Process(onion::socket::Session* session, Packet* packet)
{
	switch (packet->type())
	{
	case E_C_REQ_PING:
		delete packet;

		PK_S_ANS_PING ping;
		ping.result = 100;

		session->SendPacket(&ping);
		break;
	}
}
