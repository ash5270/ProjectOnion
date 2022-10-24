#include "PacketAnalyzer.h"
#include "../System/LogSystem.h"
Packet* onion::packet::PacketAnalyzer::Analyzer(const char* data,size_t offset,__out size_t& headerSize)
{
	if (data == nullptr || offset < 0)
		return nullptr;

	int32_t size = 0;
	int32_t id = 0;
 
	//memcpy_s(&size, sizeof(int32_t), data + offset, sizeof(int32_t));
	//offset += sizeof(int32_t);
	memcpy_s(&size, sizeof(size), data + offset, sizeof(size));
	offset += sizeof(int32_t);
	memcpy_s(&id, sizeof(int32_t), data + offset, sizeof(int32_t));
	offset += sizeof(int32_t);
	//PO_LOG(LOG_DEBUG, L"packet packetId : %d, packet size : %d\n", id,size);
	const auto packet = PacketFactory::getInstance().getPacket(id);
	headerSize = size;
	return packet;
}
