#pragma once

#define BUF_MAX_SIZE 1024

#define ZERO_MEMORY(data,size)\
	memset(data,0,size)

#include <string>
#include <stdexcept>
#include <memory>
#include <wchar.h>
#include "../System/Log.h"

template<typename... Args>
std::wstring wstring_format(const wchar_t* format,Args... args)
{
	//swprintf_s 는 nullptr이 들어가면 -1을 리턴
	//len은 가변배열을 다 받아온 길이를 가져옴
	const int len = swprintf(nullptr, 0, format, args...) + 1;
	if(len<=0)
	{
		throw std::runtime_error("Error during string format.");
	}
	auto size = static_cast<size_t>(len);
	//여기서 unique_ptr로 wchar_t 버퍼를 만들어줌
	auto buffer = std::make_unique<wchar_t[]>(size);
	_snwprintf_s(buffer.get(),size,size, format, args...);
	//여기서 buffer에 위에 받아온 가변 인자 문자열들을 복사
	//새로운 wstring 리턴
	return std::wstring(buffer.get(), buffer.get() + size - 1);
}

template<typename... Args>
std::string string_format(const char* format, Args... args)
{
	const int len = snprintf(nullptr, 0, format, args...) + 1;
	if (len <= 0)
	{
		throw std::runtime_error("Error during string format.");
	}
	auto size = static_cast<size_t>(len);
	auto buffer = std::make_unique<char[]>(size);
	snprintf(buffer.get(), size, format, args...);
	return std::string(buffer.get(), buffer.get() + size - 1);
}

static std::string MemoryToString(char* buffer, const size_t& size)
{
	std::string mem;
	char buf[6] = { 0 };
	for (int i = 0; i < size; i++)
	{
		sprintf_s(buf, 6, "%02x ", buffer[i]);
		mem.append(buf);
	}
	return mem;
}

static std::wstring MemoryToWString(char* buffer, const size_t& size)
{
	std::wstring result;
	std::string mem;
	char buf[6] = { 0 };
	for (int i = 0; i < size; i++)
	{
		sprintf_s(buf, 6, "%02x ", buffer[i]);
		mem.append(buf);
	}
	result.assign(mem.begin(), mem.end());
	return result;
}



