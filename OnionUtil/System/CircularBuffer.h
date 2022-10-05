#pragma once
#include "../Util/Common.h"
#include <memory.h>


namespace onion::system
{
	class CircularBuffer
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

		//head쪽 tail쪽 offset 더하기
		void HeadCommit(size_t len);
		void TailCommit(size_t len);

	public:
		template<typename  T>
		void operator<<(const T& value);
		void operator<<(const bool& value);
		void operator<<(const int8_t& value);
		void operator<<(const uint8_t& value);
		void operator<<(const int16_t& value);
		void operator<<(const uint16_t& value);
		void operator<<(const int32_t& value);
		void operator<<(const uint32_t& value);
		void operator<<(const int64_t& value);
		void operator<<(const uint64_t& value);
		//wstring
		void operator<<(const std::wstring& value);

		template<typename T>
		void operator>>(T* value);
		void operator>>(bool* value);
		void operator>>(int8_t* value);
		void operator>>(uint8_t* value);
		void operator>>(int16_t* value);
		void operator>>(uint16_t* value);
		void operator>>(int32_t* value);
		void operator>>(uint32_t* value);
		void operator>>(int64_t* value);
		void operator>>(uint64_t* value);
		//wstring
		void operator>>(std::wstring* value);
	};

}
