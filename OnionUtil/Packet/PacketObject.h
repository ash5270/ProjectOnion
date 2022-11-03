#pragma once
#include "../Socket/Session.h"
#include "Packet.h"

struct PacketObject
{
	onion::socket::Session* session;
	Packet* packet;
};
