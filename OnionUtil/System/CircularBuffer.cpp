#include "CircularBuffer.h"
#include <Windows.h>

onion::system::CircularBuffer::CircularBuffer(char* buffer, size_t capacity)
{
	m_data = buffer;
	m_dataEnd = m_data + capacity;
	m_capacity = capacity;
	m_headPtr = m_tailPtr = m_data;
	m_headSize = m_tailSize = 0;
	isWirteStop = false;
	isReadStop = false;
}

onion::system::CircularBuffer::CircularBuffer(size_t capacity)
{
	m_data = reinterpret_cast<char*>(
		VirtualAllocEx(GetCurrentProcess(), 0, capacity, MEM_COMMIT
			| MEM_RESERVE, PAGE_READWRITE));
	m_dataEnd = m_data + capacity;
	m_headPtr = m_tailPtr = m_data;
	m_capacity = capacity;
	m_headSize = m_tailSize = 0;
	isWirteStop = false;
	isReadStop = false;
}

onion::system::CircularBuffer::~CircularBuffer()
{
	VirtualFreeEx(GetCurrentProcess(), m_data, 0, MEM_RELEASE);
	//delete m_data;
}

bool onion::system::CircularBuffer::Write(char* value,size_t size)
{
	if(m_headPtr == m_tailPtr && isWirteStop)
	{
		return false;
	}
	//tail 데이터 끝쪽
	else 
	if(m_headPtr < m_tailPtr)
	{
		if(m_headPtr+size > m_tailPtr)
		{
			return false;
		}
	}
	//tail이 앞쪽
	else
	{
		if (m_headPtr + size > m_dataEnd)
		{
			if(m_data+size > m_tailPtr)
			{
				return false;
			}else
			{
				m_headPtr = m_data;
				m_headSize = 0;
			}
		}
	}

	//head에 데이터를 넣으면 넘치나
	/*if (m_headPtr + 1 <= m_tailPtr || m_headPtr + offset > m_tailPtr)
	{
		return false;
	}*/

	isReadStop = false;
	memcpy_s(m_headPtr, size, value, size);
	m_headPtr += size;
	m_headSize += size;

	if (m_headPtr == m_tailPtr)
		isWirteStop = true;

	return true;
}

bool onion::system::CircularBuffer::Read(char* value, size_t size)
{
	if(m_headPtr==m_tailPtr && isReadStop)
	{
		return false;
	}
	//tail 데이터 끝쪽
	else if(m_headPtr <= m_tailPtr)
	{
		if(m_tailPtr+ size  > m_dataEnd)
		{
			if(m_data +size > m_headPtr)
			{
				return false;
			}else
			{
				m_tailPtr = m_data;
				m_tailSize = 0;
			}
		}
	}
	//tail 데이터 시작점
	else
	{
		if(m_tailPtr+size> m_headPtr )
		{
			return false;
		}
	}
	//읽기 tail이 부터 읽어야 할 크기가 넘치나
	/*if (m_tailPtr + 1 >= m_headPtr || m_tailPtr + offset > m_headPtr)
	{
		return false;
	}*/


	isWirteStop = false;
	memcpy_s(value, size, m_tailPtr, size);
	m_tailPtr += size;
	m_tailSize += size;

	if (m_headPtr == m_tailPtr)
		isReadStop = true;

	return true;
}

bool onion::system::CircularBuffer::CheckReadBound(size_t len)
{
	return false;
}

bool onion::system::CircularBuffer::CheckWriteBound(size_t len)
{
	return false;
}

size_t onion::system::CircularBuffer::offset()
{
	return m_headSize;
}

size_t onion::system::CircularBuffer::size()
{
	size_t offset = 0;
	if (m_tailPtr <= m_headPtr)
	{
		offset = m_headSize - m_tailSize;
	}
	else
	{
		offset += m_tailSize;
		offset += m_headSize;
	}

	return offset;
}

size_t onion::system::CircularBuffer::capacity()
{
	return m_capacity;
}

size_t onion::system::CircularBuffer::tailOffset()
{
	return m_tailSize;
}

void onion::system::CircularBuffer::Clear()
{
	memset(m_data, 0, m_capacity);
	m_headPtr = m_data;
	m_tailPtr = m_data;
	m_headSize = 0;
	m_tailSize = 0;
}

char* onion::system::CircularBuffer::GetData() const
{
	return m_data;
}

void onion::system::CircularBuffer::HeadCommit(size_t len)
{
	m_headPtr += len;
	m_headSize += len;
}

void onion::system::CircularBuffer::TailCommit(size_t len)
{
	m_tailPtr += len;
	m_tailSize += len;
}


#define STREAM_WRITE(value)\
	if(m_headPtr == m_tailPtr && isWirteStop)\
	{\
		return ;\
	}\
	else if (m_headPtr < m_tailPtr)\
	{\
		if (m_headPtr + sizeof(value) > m_tailPtr)\
		{\
			return ;\
		}\
	}\
	else\
	{\
		if (m_headPtr + sizeof(value) > m_dataEnd)\
		{\
			if (m_data + sizeof(value) > m_tailPtr)\
			{\
				return ;\
			}\
			else\
			{\
				m_headPtr = m_data;\
				m_headSize = 0;\
			}\
		}\
	}\
isReadStop = false;\
memcpy_s(m_headPtr, sizeof(value), &value, sizeof(value));\
m_headPtr += sizeof(value);\
m_headSize += sizeof(value);\
if (m_headPtr == m_tailPtr)\
	isWirteStop = true;\
\
return\

//---------write-------
void onion::system::CircularBuffer::operator<<(const bool& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const int8_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const uint8_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const int16_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const uint16_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const int32_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const uint32_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const int64_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const uint64_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const std::wstring& value)
{
	*this << (int32_t)(value.length());
	for (auto i : value)
		*this << i;
	return;
}
#define STREAM_READ(type,value)\
	auto value_ptr = reinterpret_cast<char*>(value);\
	auto offset = sizeof(type);\
	Read(value_ptr, offset)\
//-------read------
template <typename T>
void onion::system::CircularBuffer::operator>>(T* value)
{
	STREAM_READ(T,value);
}

void onion::system::CircularBuffer::operator>>(bool* value)
{
	STREAM_READ(bool,value);
}

void onion::system::CircularBuffer::operator>>(int8_t* value)
{
	STREAM_READ(int8_t,value);
}

void onion::system::CircularBuffer::operator>>(uint8_t* value)
{
	STREAM_READ(uint8_t,value);
}

void onion::system::CircularBuffer::operator>>(int16_t* value)
{
	STREAM_READ(int16_t,value);
}

void onion::system::CircularBuffer::operator>>(uint16_t* value)
{
	STREAM_READ(uint16_t,value);
}

void onion::system::CircularBuffer::operator>>(int32_t* value)
{
	STREAM_READ(int32_t,value);
}

void onion::system::CircularBuffer::operator>>(uint32_t* value)
{
	STREAM_READ(uint32_t,value);
}

void onion::system::CircularBuffer::operator>>(int64_t* value)
{
	STREAM_READ(int64_t,value);
}

void onion::system::CircularBuffer::operator>>(uint64_t* value)
{
	STREAM_READ(uint64_t,value);
}

void onion::system::CircularBuffer::operator>>(std::wstring* value)
{
	int32_t size = 0;
	*this >> (int32_t*)& size;

	wchar_t* buffer = new wchar_t[size + 1];

	this->Read(reinterpret_cast<char*>(buffer), sizeof(wchar_t) * size);
	buffer[size] = '\0';

	value->clear();
	*value = buffer;
	delete[] buffer;
}

template <typename T>
void onion::system::CircularBuffer::operator<<(const T& value)
{
	STREAM_WRITE(value);
}
