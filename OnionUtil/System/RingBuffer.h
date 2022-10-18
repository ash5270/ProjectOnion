﻿#pragma once
#include "../Util/Common.h"
#include "Stream.h"
#include "../Packet/PacketID.h"
namespace onion::system
{
	class RingBuffer :public  Stream
	{
	private:
		char* m_data;
		char* m_dataEnd;

		char* m_headPtr;
		size_t m_headSize;

		char* m_tailPtr;
		size_t m_tailSize;

		size_t m_capcity;

		bool Write(const void* data, size_t size);
		bool Read(__out char* data, size_t size);

	public:
		RingBuffer(size_t capacity);
		~RingBuffer();

		void Clear();

		void Remove(size_t len);

		size_t GetFreeSpaceSize();
		size_t GetStoredSize() const;

		size_t GetContiguiousBytes() const;

		size_t GetWritableOffset() const;
		void Commit(size_t len);
		size_t GetReadableOffset()const;

		size_t GetCapcity()const;

		char* GetData() const;
	private:

		void AllocateTail();

		size_t GetHeadFreeSize()const;
		size_t GetSpaceBeforeHead()const;
		size_t GetTailFreeSpace() const;

	public:
		template<typename  T>
		void operator<<(const T& value);
		void operator<<(const bool& value) override;
		void operator<<(const int8_t& value) override;
		void operator<<(const uint8_t& value) override;
		void operator<<(const int16_t& value) override;
		void operator<<(const uint16_t& value) override;
		void operator<<(const int32_t& value) override;
		void operator<<(const uint32_t& value) override;
		void operator<<(const int64_t& value) override;
		void operator<<(const uint64_t& value) override;
		//wstring
		void operator<<(const std::wstring& value) override;
		void operator<<(PacketHeader*& header)override;

		template<typename T>
		void operator>>(T* value);
		void operator>>(bool* value) override;
		void operator>>(int8_t* value) override;
		void operator>>(uint8_t* value) override;
		void operator>>(int16_t* value)override;
		void operator>>(uint16_t* value)override;
		void operator>>(int32_t* value)override;
		void operator>>(uint32_t* value)override;
		void operator>>(int64_t* value)override;
		void operator>>(uint64_t* value)override;
		//wstring
		void operator>>(std::wstring* value)override;
	};


}
