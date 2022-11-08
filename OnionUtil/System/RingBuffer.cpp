#include "RingBuffer.h"
#include <Windows.h>



onion::system::RingBuffer::RingBuffer(size_t capacity):m_capcity(capacity)
{
	m_data= reinterpret_cast<char*>(VirtualAllocEx(GetCurrentProcess(), 0, capacity, MEM_COMMIT
		| MEM_RESERVE, PAGE_READWRITE));
	m_dataEnd = m_data + capacity;
	Clear();
}

onion::system::RingBuffer::~RingBuffer()
{
	VirtualFreeEx(GetCurrentProcess(), m_data, 0, MEM_RELEASE);
}

void onion::system::RingBuffer::Clear()
{
	m_headPtr = m_data;
	m_tailPtr = nullptr;
	m_headSize = 0;
	m_tailSize = 0;
}

bool onion::system::RingBuffer::Write(const void* data, size_t size)
{
	//tail 영역이 있다면 tail영역에 먼저 쓴다
	if(m_tailPtr!=nullptr)
	{
		if (GetTailFreeSpace() < size)
			return false;
		memcpy_s(m_tailPtr + m_tailSize, size, data, size);
		m_tailSize += size;
		return true;
	}
	//head 영역 보다 tail영역의 용량이 더 클 경우 그 영역을 tail로 설정하고 쓰기
	if(GetHeadFreeSize()<GetSpaceBeforeHead())
	{
		AllocateTail();

		if(GetTailFreeSpace()<size)
		{
			return	false;
		}
		memcpy_s(m_tailPtr + m_tailSize, size, data, size);
		m_tailSize += size;
		return true;
	}
	//head 공간이 더 크면 앞에 다 쓰기
	else
	{
		if (GetHeadFreeSize() < size)
			return false;

		memcpy_s(m_headPtr + m_headSize, size, data, size);
		m_headSize += size;
		return true;
	}
}

bool onion::system::RingBuffer::Read(char* data, size_t size)
{
	//
	if (m_headSize + m_tailSize < size)
		return false;

	size_t count = size;
	size_t read_size = 0;
	//Read와 똑같기 떄문에 A가 있다면 A영역부터 삭제
	if (m_headSize > 0)
	{
		//지워야할 사이즈가 head 사이즈 보다 크면 head 사이즈부터 일단 지움
		read_size = (count > m_headSize) ? m_headSize : count;
		memcpy_s(data, read_size, m_headPtr, read_size);
		m_headSize -= read_size;
		m_headPtr += read_size;
		count -= read_size;
	}
	//제거할게 아직 남아 있다면
	if (count > 0 && m_tailSize > 0)
	{
		read_size = count;
		memcpy_s(data, read_size, m_tailPtr, read_size);

		m_tailSize -= read_size;
		m_tailPtr += read_size;
		count -= read_size;
	}
	//head 영역이 비워지면 B로 바꿈
	if (m_headSize == 0)
	{
		if (m_tailSize > 0)
		{
			//앞으로 당기기
			if (m_tailPtr != m_data)
			{
				memmove(m_data, m_tailPtr + 1, m_tailSize);
				printf("move\n");
			}


			m_headSize = m_tailSize;
			m_headPtr = m_data;
			m_tailPtr = nullptr;
			m_tailSize = 0;
		}
		else
		{
			m_tailPtr = nullptr;
			m_tailSize = 0;
			printf("당김3\n");
			m_headPtr = m_data;
			m_headSize = 0;
		}
	}

	return true;
}


void onion::system::RingBuffer::Remove(size_t len)
{
	size_t count = len;
	//Read와 똑같기 떄문에 A가 있다면 A영역부터 삭제
	if(m_headSize>0)
	{
		//지워야할 사이즈가 head 사이즈 보다 크면 head 사이즈부터 일단 지움
		size_t remove_size = (count > m_headSize) ? m_headSize : count;
		m_headSize -= remove_size;
		m_headPtr += remove_size;
		count -= remove_size;
	}
	//제거할게 아직 남아 있다면
	if(count>0 && m_tailSize>0)
	{
		size_t remove_size = (count > m_tailSize) ? m_tailSize : count;
		m_tailSize -= remove_size;
		m_tailPtr += remove_size;
		count -= remove_size;
	}
	//head 영역이 비워지면 B로 바꿈
	if(m_headSize==0)
	{
		if(m_tailSize>0)
		{
			//앞으로 당기기
			if (m_tailPtr != m_data)
			{
				memmove(m_data, m_tailPtr, m_tailSize);
			}

			m_headSize = m_tailSize;
			m_headPtr = m_data;
			m_tailPtr = nullptr;
			m_tailSize = 0;
		}else
		{
			m_tailPtr = nullptr;
			m_tailSize = 0;
			m_headPtr = m_data;
			m_headSize = 0;
		}
	}
}

size_t onion::system::RingBuffer::GetFreeSpaceSize()
{
	if (m_tailPtr != nullptr)
		//tail쪽이 포인터가 있다면 tail쪽 비어있는 공간 사이즈 
		return GetTailFreeSpace();
	else
	{
		//
		if(GetHeadFreeSize()<GetSpaceBeforeHead())
		{
			//
			AllocateTail();
			//
			return GetTailFreeSpace();
		}
		else
			//
			return GetHeadFreeSize();
	}
}

size_t onion::system::RingBuffer::GetStoredSize() const
{
	return m_headSize + m_tailSize;
}

size_t onion::system::RingBuffer::GetContiguiousBytes() const
{
	//지금 사용하고 있는 사이즈 주기
	if (m_headSize > 0)
		return m_headSize;
	else
		return m_tailSize;
}

size_t onion::system::RingBuffer::GetWritableOffset() const
{
	if (m_tailPtr != nullptr)
		//tail pointer에 사이즈 더하고 m_data로 뺀것 
		//처음 가로가 offset위치 거기서 처음 데이터를 빼면 offset만 남음
		return static_cast<size_t>((m_tailPtr + m_tailSize) - m_data);
	else
		return static_cast<size_t>(m_headPtr + m_headSize - m_data);
}

void onion::system::RingBuffer::Commit(size_t len)
{
	if (m_tailPtr != nullptr)
		m_tailSize += len;
	else
		m_headSize += len;
}

size_t onion::system::RingBuffer::GetReadableOffset() const
{
	//head쪽에 데이터가 있다면 그쪽부터 읽어라
	if (m_headSize > 0 || m_tailPtr==nullptr )
		//근데 offset 읽는곳 부터 즉 쓰여진 곳 부터니깐 
		//m_headPtr - m_data로 offset만 남음
		return static_cast<size_t>(m_headPtr - m_data);
	else
		return static_cast<size_t>(m_tailPtr - m_data);
}

size_t onion::system::RingBuffer::GetCapcity() const
{
	return m_capcity;
}

char* onion::system::RingBuffer::GetData() const
{
	return m_data;
}

void onion::system::RingBuffer::AllocateTail()
{
	m_tailPtr = m_data;
}

size_t onion::system::RingBuffer::GetHeadFreeSize() const
{
	return (m_dataEnd - m_headPtr - m_headSize);
}

size_t onion::system::RingBuffer::GetSpaceBeforeHead() const
{
	return (m_headPtr - m_data);
}

size_t onion::system::RingBuffer::GetTailFreeSpace() const
{
	if (m_tailPtr == nullptr)
		return 0;
	return (m_dataEnd - m_tailPtr - m_tailSize);
}

void onion::system::RingBuffer::operator<<(const bool& value)
{
	Write(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator<<(const int8_t& value)
{
	Write(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator<<(const uint8_t& value)
{
	Write(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator<<(const int16_t& value)
{
	Write(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator<<(const uint16_t& value)
{
	Write(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator<<(const int32_t& value)
{
	Write(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator<<(const uint32_t& value)
{
	Write(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator<<(const int64_t& value)
{
	Write(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator<<(const uint64_t& value)
{
	Write(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator<<(const float& value)
{
	Write(&value, sizeof(value));
}

void onion::system::RingBuffer::operator<<(const double& value)
{
	Write(&value, sizeof(value));
}

void onion::system::RingBuffer::operator<<(const std::wstring& value)
{
	auto size = (int32_t)value.capacity()+1;
	if(Write(reinterpret_cast<char*>(&size),sizeof(uint32_t)))
		Write((char*)value.c_str(), size);
}

void onion::system::RingBuffer::operator<<(PacketHeader*& header)
{
	header = reinterpret_cast<PacketHeader*>(m_data + GetWritableOffset());
	Write(header, sizeof(PacketHeader));
}




template <typename T>
void onion::system::RingBuffer::operator>>(T* value)
{
	Read(value, sizeof(value));
}

void onion::system::RingBuffer::operator>>(bool* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(bool));
}

void onion::system::RingBuffer::operator>>(int8_t* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(int8_t));
}

void onion::system::RingBuffer::operator>>(uint8_t* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(uint8_t));
}

void onion::system::RingBuffer::operator>>(int16_t* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(int16_t));
}

void onion::system::RingBuffer::operator>>(uint16_t* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(uint16_t));
}

void onion::system::RingBuffer::operator>>(int32_t* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(int32_t));
}

void onion::system::RingBuffer::operator>>(uint32_t* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(uint32_t));
}

void onion::system::RingBuffer::operator>>(int64_t* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(int64_t));
}

void onion::system::RingBuffer::operator>>(uint64_t* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(uint64_t));
}

void onion::system::RingBuffer::operator>>(std::wstring* value)
{
	int32_t size = 0;
	if(Read(reinterpret_cast<char*>(&size),sizeof(int32_t)))
	{
		wchar_t* buffer = new wchar_t[size + 1];

		if(this->Read(reinterpret_cast<char*>(buffer), sizeof(wchar_t) * size))
		{
			buffer[size] = '\0';

			value->clear();
			*value = buffer;
			delete[] buffer;
		}else
		{
			printf("error 2\n");
		}
		
	}else
	{
		printf("error\n");
	}
}



void onion::system::RingBuffer::operator>>(float* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(value));
}

void onion::system::RingBuffer::operator>>(double* value)
{
	Read(reinterpret_cast<char*>(value), sizeof(value));
}


template <typename T>
void onion::system::RingBuffer::operator<<(const T& value)
{
	Read(reinterpret_cast<char*>(value), sizeof(T));
}
