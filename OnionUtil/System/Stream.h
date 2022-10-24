#pragma once
#include <cstdint>
#include <string>
#include "../Packet/PacketID.h"
//buffer의 부모

namespace onion::system
{
	class Stream
	{
	public:
		/*template<typename  T>
		virtual void operator<<(const T& value){}*/
		inline virtual void operator<<(const bool& value) = 0;
		inline virtual  void operator<<(const int8_t& value) = 0;
		inline virtual void operator<<(const uint8_t& value) = 0;
		inline virtual void operator<<(const int16_t& value) = 0;
		inline virtual void operator<<(const uint16_t& value) = 0;
		inline virtual void operator<<(const int32_t& value) = 0;
		inline virtual void operator<<(const uint32_t& value) = 0;
		inline virtual void operator<<(const int64_t& value) = 0;
		inline virtual void operator<<(const uint64_t& value) = 0;
		 //wstring
		inline virtual void operator<<(const std::wstring& value) = 0;
		inline virtual void operator<<(PacketHeader*& header) = 0;
		 
		 //template<typename T>
		 //void operator>>(T* value);
		inline virtual void operator>>(bool* value) = 0;
		inline virtual void operator>>(int8_t* value) = 0;
		inline virtual void operator>>(uint8_t* value) = 0;
		inline virtual void operator>>(int16_t* value) = 0;
		inline virtual void operator>>(uint16_t* value) = 0;
		inline virtual void operator>>(int32_t* value) = 0;
		inline virtual void operator>>(uint32_t* value) = 0;
		inline virtual void operator>>(int64_t* value) = 0;
		inline virtual void operator>>(uint64_t* value) = 0;
		 //wstring											
		inline virtual void operator>>(std::wstring* value) = 0;
	};
}