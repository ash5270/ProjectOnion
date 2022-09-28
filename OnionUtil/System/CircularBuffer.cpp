#include "CircularBuffer.h"

void onion::system::CircularBuffer::Write(char* value,size_t size)
{
	//memcpy_s(m_headPtr,size,)	
}

void onion::system::CircularBuffer::Read(void* value, size_t size)
{
	
}

onion::system::CircularBuffer::CircularBuffer(char* buffer, size_t capacity)
{

}

onion::system::CircularBuffer::~CircularBuffer()
{

}

bool onion::system::CircularBuffer::CheckReadBound(size_t len)
{
	return false;
}

bool onion::system::CircularBuffer::CheckWriteBound(size_t len)
{
	return false;
}

void onion::system::CircularBuffer::Clear()
{
	
}

char* onion::system::CircularBuffer::GetData() const
{
	return m_data;
}



#define STREAM_WRITE(value)\
	int32_t size =sizeof(value);\
	if(!this->CheckWriteBound(size))\
		return;\

//---------write-------
void onion::system::CircularBuffer::operator<<(const bool& value)
{

}

void onion::system::CircularBuffer::operator<<(const int8_t& value)
{

}

void onion::system::CircularBuffer::operator<<(const uint8_t& value)
{

}

void onion::system::CircularBuffer::operator<<(const int16_t& value)
{

}

void onion::system::CircularBuffer::operator<<(const uint16_t& value)
{

}

void onion::system::CircularBuffer::operator<<(const int32_t& value)
{

}

void onion::system::CircularBuffer::operator<<(const uint32_t& value)
{

}

void onion::system::CircularBuffer::operator<<(const int64_t& value)
{

}

void onion::system::CircularBuffer::operator<<(const uint64_t& value)
{

}

void onion::system::CircularBuffer::operator<<(const std::wstring& value)
{

}

//-------read------
template <typename T>
void onion::system::CircularBuffer::operator>>(T* value)
{

}

void onion::system::CircularBuffer::operator>>(bool* value)
{

}

void onion::system::CircularBuffer::operator>>(int8_t* value)
{

}

void onion::system::CircularBuffer::operator>>(uint8_t* value)
{

}

void onion::system::CircularBuffer::operator>>(int16_t* value)
{

}

void onion::system::CircularBuffer::operator>>(uint16_t* value)
{

}

void onion::system::CircularBuffer::operator>>(int32_t* value)
{

}

void onion::system::CircularBuffer::operator>>(uint32_t* value)
{

}

void onion::system::CircularBuffer::operator>>(int64_t* value)
{

}

void onion::system::CircularBuffer::operator>>(uint64_t* value)
{

}

void onion::system::CircularBuffer::operator>>(std::wstring* value)
{

}

template <typename T>
void onion::system::CircularBuffer::operator<<(const T& value)
{

}
