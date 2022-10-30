#pragma once
#include "../Util/Common.h"
#include <memory.h>
#include "Stream.h"
#include "Buffer.h"


namespace onion::system
{
	class CircularBuffer : public Stream
	{
		//start
		char* m_data;
		//end
		char* m_dataEnd;
		//head_ptr
		char* m_headPtr;
		size_t m_headSize;
		//tail_ptr
		char* m_tailPtr;
		size_t m_tailSize;
		//버퍼 넘쳤을 경우 쓰기 금지
		bool isWirteStop;
		bool isReadStop;

		bool Write(char* value, size_t size);
		bool Read(char* value, size_t size);

		size_t m_capacity;
	public:
		CircularBuffer(char* buffer, size_t capacity);
		CircularBuffer(size_t capacity);
		~CircularBuffer();

		int write_size;
		
		bool CheckReadBound(size_t len);
		bool CheckWriteBound(size_t len);

		//offset
		size_t offset();
		size_t size();
		//capacity
		size_t capacity();
		size_t tailOffset();
		//Clear
		void Clear();
		//data
		char* GetData() const;
		char* GetTail() const;

		//head쪽 tail쪽 offset 더하기
		void HeadCommit(size_t len);
		void TailCommit(size_t len);

		//앞쪽으로 남아있는 만큼 땡김
		void Remove(size_t len);
		//tail까지 쭉 비우기
		void HeadClear();

	public:
		bool operator<<(Buffer& buffer)
		{
			if (m_capacity < m_headSize + buffer.size())
			{
				return false;
			}

			memcpy_s(m_headPtr, m_capacity - m_headSize, buffer.GetData(), buffer.size());
			m_headPtr += buffer.size();
			m_headSize += buffer.size();
			return  true;
		}

		template<typename  T>
		inline void operator<<(const T& value);
		inline void operator<<(const bool& value) override;
		inline void operator<<(const int8_t& value) override;
		inline void operator<<(const uint8_t& value) override;
		inline void operator<<(const int16_t& value) override;
		inline void operator<<(const uint16_t& value) override;
		inline void operator<<(const int32_t& value) override;
		inline void operator<<(const uint32_t& value) override;
		inline void operator<<(const int64_t& value) override;
		inline void operator<<(const uint64_t& value) override;

		inline //wstring
		inline void operator<<(const std::wstring& value) override;
		inline void operator<<(PacketHeader*& header)override;
		

		template<typename T>
		inline void operator>>(T* value);
		inline void operator>>(bool* value) override;
		inline void operator>>(int8_t* value) override;
		inline void operator>>(uint8_t* value) override;
		inline void operator>>(int16_t* value) override;
		inline void operator>>(uint16_t* value) override;
		inline void operator>>(int32_t* value) override;
		inline void operator>>(uint32_t* value) override;
		inline void operator>>(int64_t* value) override;
		inline void operator>>(uint64_t* value) override;

		//wstring
		inline void operator>>(std::wstring* value) override;

	};

}
