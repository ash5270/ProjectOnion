#pragma once
#include "../Util/Common.h"
#include <string>
#include <iostream>
namespace onion::system
{
	class Buffer
	{
		char* m_data;
		size_t m_offset;
		size_t m_readOffset;
		size_t m_capacity;

	public:
		Buffer();
		Buffer(size_t size);
		Buffer(const Buffer& buffer, size_t size);
		//�ٸ� ���ۿ��� ������ �����ϴ°� �ƴ�
		Buffer(char* setbuf, size_t size);

		~Buffer();
		
		void Clear();
		char* GetData() const;
		//add offset -> ��������� recv ������ ���� ��ü�� �����ؿ����� �� offset���� �������� �ʱ� ����
		void AddOffset(size_t offset);
		//size
		size_t size();
		//capacity
		size_t capacity();

		//check bound
		bool CheckWriteBound(size_t len);
		bool CheckReadBound(size_t len);

		//write
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
		//void operator<<(const Buffer& buffer);
		//wstring
		void operator<<(const std::wstring& value);

		//read
		void Read(void* value, size_t size);

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
