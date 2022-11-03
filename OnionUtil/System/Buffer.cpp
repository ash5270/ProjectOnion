#include "Buffer.h"
#include "LogSystem.h"
#include "../Packet/PacketID.h"

onion::system::Buffer::Buffer()
{
	m_data = new char[BUF_MAX_SIZE];
	m_capacity = BUF_MAX_SIZE;
	Clear();
}

onion::system::Buffer::Buffer(size_t size)
{
	m_data = new char[size];
	m_capacity = size;
	Clear();
}

onion::system::Buffer::Buffer(const Buffer& buffer, size_t size)
{
	m_readOffset = buffer.m_readOffset;
	m_offset = buffer.m_offset;
	m_capacity = buffer.m_capacity;
	m_data = new char[size];
	//여기서 복사 하기
}

onion::system::Buffer::Buffer(char* setbuf, size_t size)
{
	m_data = setbuf;
	m_capacity = size;
	Clear();
}

onion::system::Buffer::~Buffer()
{
	if (m_data != nullptr)
	{
		delete[] m_data;
		m_data = nullptr;
	}
}

void onion::system::Buffer::Clear()
{
	//PO_LOG(LOG_ERROR,L"초기화 buffer\n");
	m_offset = 0;
	write_size = 0;
	m_readOffset = 0;
	if(m_data==nullptr)
		return;
	ZERO_MEMORY(m_data, m_capacity);
}

char* onion::system::Buffer::GetData() const
{
	return m_data;
}

void onion::system::Buffer::AddOffset(size_t offset)
{
	if(m_capacity < m_offset + offset)
		return;
	m_offset += offset;
}

void onion::system::Buffer::Commit(size_t size)
{
	memmove(m_data, m_data + size, m_offset);
 	m_offset -= size;
}

size_t onion::system::Buffer::size()
{
	return m_offset;
}

size_t onion::system::Buffer::capacity()
{
	return m_capacity;
}

bool onion::system::Buffer::CheckWriteBound(size_t len)
{
	if (m_offset + len > m_capacity)
	{
		PO_LOG(LOG_ERROR, L"buffer Write overflow\n");
		return false;
	}
	return true;
}

bool onion::system::Buffer::CheckReadBound(size_t len) 
{
	if(m_readOffset+len>m_offset)
	{
		PO_LOG(LOG_ERROR, L"buffer read overflow\n");
		return false;
	}
	return true;
}

#define STREAM_WRITE(value)\
	int32_t size= sizeof(value);\
	if(!this->CheckWriteBound(size))\
		return;\
	memcpy_s((m_data)+m_offset,m_capacity-m_offset,(const void*)&value,size);\
	m_offset+=size;\
	write_size+=size

template <typename T>
void onion::system::Buffer::operator<<(const T& value)
{
	STREAM_WRITE(value);
}

void onion::system::Buffer::operator<<(const bool& value)
{
	STREAM_WRITE(value);
}

void onion::system::Buffer::operator<<(const int8_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::Buffer::operator<<(const uint8_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::Buffer::operator<<(const int16_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::Buffer::operator<<(const uint16_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::Buffer::operator<<(const int32_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::Buffer::operator<<(const uint32_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::Buffer::operator<<(const int64_t& value)
{
	STREAM_WRITE(value);
}

void onion::system::Buffer::operator<<(const uint64_t& value)
{
	STREAM_WRITE(value);
}

//bool onion::system::Buffer::operator<<(const Buffer& buffer)
//{
//	
//}

void onion::system::Buffer::operator<<(const std::wstring& value)
{
	*this << (int32_t)(value.length());
	//for (auto i : value)  wchar_t -> 
	//	*this << i;
	if (!CheckWriteBound(value.length()))
		return;
	memcpy(m_data + m_offset, value.c_str(), value.size() * sizeof(wchar_t));
	m_offset += value.size() * sizeof(wchar_t);
	write_size += value.size() * sizeof(wchar_t);
	return;



}

void onion::system::Buffer::operator<<(PacketHeader*&header)
{
	if(!CheckWriteBound(sizeof(PacketHeader)))
	{
		return;
	}

	header = reinterpret_cast<PacketHeader*>(m_data + m_offset);
	m_offset += sizeof(PacketHeader);
	
	/*write_size += sizeof(PacketHeader);*/
}	

void onion::system::Buffer::Read(void* value, size_t size)
{
	memcpy_s(value, size, (void*)(m_data + m_readOffset), size);
	m_readOffset += size;
}

#define STREAM_READ(type,value)\
	size_t size = sizeof(type);\
	if(!this->CheckReadBound(size))\
		return;\
	this->Read((void*)value,size);


template <typename T>
void onion::system::Buffer::operator>>(T* value)
{
	STREAM_READ(T,value);
}

void onion::system::Buffer::operator>>(bool* value)
{
	STREAM_READ(bool, value);
}

void onion::system::Buffer::operator>>(int8_t* value)
{
	STREAM_READ(int8_t, value);
}

void onion::system::Buffer::operator>>(uint8_t* value)
{
	STREAM_READ(uint8_t, value);
}

void onion::system::Buffer::operator>>(int16_t* value)
{
	STREAM_READ(int16_t, value);
}

void onion::system::Buffer::operator>>(uint16_t* value)
{
	STREAM_READ(uint16_t, value);
}

void onion::system::Buffer::operator>>(int32_t* value)
{
	STREAM_READ(int32_t, value);
}

void onion::system::Buffer::operator>>(uint32_t* value)
{
	STREAM_READ(uint32_t, value);
}

void onion::system::Buffer::operator>>(int64_t* value)
{
	STREAM_READ(int64_t, value);
}

void onion::system::Buffer::operator>>(uint64_t* value)
{
	STREAM_READ(uint64_t, value);
}

void onion::system::Buffer::operator>>(std::wstring* value)
{
	int32_t size = 0;
	*this >> &size;
	if(size<=0||!CheckReadBound(size))
		return;
	wchar_t* buffer = new wchar_t[size + 1];
	this->Read((void*)(buffer), sizeof(wchar_t) * size);
	buffer[size] = '\0';

	value->clear();
	*value = buffer;

	delete[] buffer;
}