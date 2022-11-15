//packet을 저장하기위해 사용할 데이터
//session과 packet을 들고 있음
#pragma once
#include "../Socket/Session.h"
#include "Packet.h"

struct PacketObject
{
	onion::socket::Session* session;
	Packet* packet;
};
