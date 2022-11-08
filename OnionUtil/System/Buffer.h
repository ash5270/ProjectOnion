#pragma once
#include "../Util/Common.h"
#include "../Packet/PacketID.h"
#include "Stream.h"

namespace onion::system
{
	class Buffer :public Stream
	{
		char* m_data;
		size_t m_offset;
		size_t m_readOffset;
		size_t m_capacity;
	public:
		size_t write_size;
	public:
		Buffer();
		Buffer(size_t size);
		Buffer(const Buffer& buffer, size_t size);
		//다른 버퍼에서 데이터 복사하는게 아님
		Buffer(char* setbuf, size_t size);
		~Buffer();

		void Clear();
		char* GetData() const;
		//add offset -> 사용이유는 recv 했을때 버퍼 자체를 복사해오지만 이 offset값은 복사하지 않기 때문
		void AddOffset(size_t offset);
		void Commit(size_t size);
		//offset
		size_t size();
		//capacity
		size_t capacity();

		//check bound
		bool CheckWriteBound(size_t len);
		bool CheckReadBound(size_t len);

		//write
		template<typename  T>
		void operator<<(const T& value);
		inline	void operator<<(const bool& value) override;
		inline	void operator<<(const int8_t& value)override;
		inline	void operator<<(const uint8_t& value)override;
		inline	void operator<<(const int16_t& value)override;
		inline	void operator<<(const uint16_t& value)override;
		inline	void operator<<(const int32_t& value)override;
		inline	void operator<<(const uint32_t& value)override;
		inline	void operator<<(const int64_t& value)override;
		inline	void operator<<(const uint64_t& value)override;
		inline void operator<<(const float& value) override;
		inline void operator<<(const double& value) override;
		bool operator<<(const Buffer& buffer)
		{
			if (m_capacity < m_offset + buffer.m_offset)
			{
				//PO_LOG(LOG_ERROR, L"Buffer overflow \n");
				return false;
			}

			memcpy_s(m_data + m_offset, m_capacity - m_offset, buffer.m_data, buffer.m_offset);
			m_offset += buffer.m_offset;
			return true;
		}
		//wstring
		inline	void operator<<(const std::wstring& value)override;
		inline	void operator<<(PacketHeader*& header) override;

		//read
		inline	void Read(void* value, size_t size);

		template<typename T>
		void operator>>(T* value);
		inline	void operator>>(bool* value) override;
		inline	void operator>>(int8_t* value) override;
		inline	void operator>>(uint8_t* value) override;
		inline	void operator>>(int16_t* value) override;
		inline	void operator>>(uint16_t* value) override;
		inline	void operator>>(int32_t* value) override;
		inline	void operator>>(uint32_t* value) override;
		inline	void operator>>(int64_t* value) override;
		inline	void operator>>(uint64_t* value) override;
		//wstring
		inline	void operator>>(std::wstring* value) override;
		inline void operator>>(float* value) override;
		inline void operator>>(double* value) override;
	};

}
