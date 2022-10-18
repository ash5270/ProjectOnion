#pragma once
#include "PacketFactory.h"
#include "../Util/SingleTon.h"

namespace onion::packet
{
	//패킷 분석기
	//말 그대로 패킷을 분석해주는거다.
	//
	class PacketAnalyzer : public SingleTon<PacketAnalyzer>
	{
	public:
		/**
		 * \brief 패킷 분석하고 그에 맞는 패킷 리턴
		 * \param data 버퍼
		 * \param offset 버퍼로 부터 읽을 offset
		 * \return new packet pointer
		 */
		Packet* Analyzer(const char* data,size_t offset, __out size_t& headerSize);
	};

}
