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


		void Write(char* value, size_t size);
		void Read(void* value, size_t size);

	public:
		CircularBuffer(char* buffer, size_t capacity);
		~CircularBuffer();

		bool CheckReadBound(size_t len);
		bool CheckWriteBound(size_t len);

		void Clear();
		char* GetData() const;

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
