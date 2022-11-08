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
	//리드만 버퍼를 넘어가면 읽을 수 있게 만듬
	//Rio가 어쩔 수 없이 버퍼를 다 쓰면 앞에 쪽에 쓰게 됨!
	
	//읽은 데이터 사이즈 
	int readSize = 0;
	int dataSize = size;
	//데이터를 아예 읽을 수 없을때 
	if(m_headPtr==m_tailPtr && isReadStop)
	{
		return false;
	}
	//tail 데이터 끝쪽
	else if(m_headPtr <= m_tailPtr)
	{
		//데이터 남은게 있다면 
		if(m_tailPtr+ size  > m_dataEnd)
		{
			size_t endSize = m_dataEnd - m_tailPtr;
			readSize += endSize;
			memcpy_s(value, endSize, m_tailPtr, endSize);

			m_tailPtr = m_data;
			m_tailSize += endSize;
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
	memcpy_s(value, dataSize-readSize, m_tailPtr, dataSize-readSize);
	m_tailPtr += dataSize;
	m_tailSize += dataSize;

	if (m_headPtr == m_tailPtr)
		isReadStop = true;

	return true;
}

bool onion::system::CircularBuffer::CheckReadBound(size_t len)
{
	if(m_tailPtr+m_tailSize+len>m_dataEnd)
	{
		return false;
	}else
	{
		return true;
	}
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
		offset += m_capacity - m_tailSize;
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

char* onion::system::CircularBuffer::GetTail() const
{
	return m_tailPtr;
}

void onion::system::CircularBuffer::HeadCommit(size_t len)
{
	if(m_headPtr+len > m_dataEnd)
	{
		m_headPtr = m_data;
		m_headSize = 0;
	}
	m_headPtr += len;
	m_headSize += len;
}

void onion::system::CircularBuffer::TailCommit(size_t len)
{
	if(m_tailPtr+len > m_dataEnd)
	{
		m_tailPtr = m_data;
		m_tailSize = 0;
	}
	m_tailPtr += len;
	m_tailSize += len;
}

void onion::system::CircularBuffer::Remove(size_t len)
{
	//remove
	//데이터 앞쪽으로 옮김
	if(len<0)
		return;
	memmove(m_data,m_tailPtr, m_headSize - m_tailSize);
	m_headSize = m_headSize - m_tailSize;
	m_tailPtr = m_data;
	m_tailSize = 0;
	m_headPtr = m_data + m_headSize;
	memset(m_data + m_headSize, 0, m_capacity - m_headSize);
}

void onion::system::CircularBuffer::HeadClear()
{
	m_headPtr = m_data;
	m_headSize = 0;
	m_tailPtr = 0;
	m_tailSize = 0;
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

void onion::system::CircularBuffer::operator<<(const float& value)
{
	STREAM_WRITE(value);
}

void onion::system::CircularBuffer::operator<<(const double& value)
{
	STREAM_WRITE(value);
}



void onion::system::CircularBuffer::operator<<(const std::wstring& value)
{
	//char 사이즈가 아니기 때문에 capacity + 1 
	auto size = (int32_t)value.size()*sizeof(wchar_t);
	STREAM_WRITE(size);
	Write((char*)value.c_str(), size);
}

void onion::system::CircularBuffer::operator<<(PacketHeader*& header)
{
	//할당
	header = reinterpret_cast<PacketHeader*>(m_data + m_headSize);
	Write(m_data + m_headSize, sizeof(PacketHeader));
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
	if(size<=0)
	{
		PO_LOG(LOG_ERROR, L"wstring write error : %d", size);
		return;
	}
	wchar_t* buffer = new wchar_t[size/(sizeof(wchar_t)) + 1];
	this->Read((char*)(buffer), size);
	buffer[size / (sizeof(wchar_t))] = '\0';
	value->clear();
	*value = buffer;
	delete[] buffer;
}

void onion::system::CircularBuffer::operator>>(float* value)
{
	STREAM_READ(float, value);
}

void onion::system::CircularBuffer::operator>>(double* value)
{
	STREAM_READ(double, value);
}

template <typename T>
void onion::system::CircularBuffer::operator<<(const T& value)
{
	STREAM_WRITE(value);
}

