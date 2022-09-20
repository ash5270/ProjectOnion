﻿#include "LogSystem.h"

void onion::system::LogSystem::Start()
{
	m_isUpdate = true;
	m_thread = std::thread(&LogSystem::Update, this);
	//error
	setlocale(LC_ALL, "");
	std::wcout.imbue(std::locale(""));
}

void onion::system::LogSystem::Stop()
{
	m_isUpdate = false;
	if (m_thread.joinable())
		m_thread.join();
}

void onion::system::LogSystem::AddLog(LogData&& logData)
{
	//lock
	std::lock_guard<std::mutex> lock(m_lock);
	m_logDatas.push(std::move(logData));
}

void onion::system::LogSystem::Update()
{
	while (m_isUpdate)
	{
		if (!m_logDatas.empty())
		{
			//lock
			std::lock_guard<std::mutex> lock(m_lock);

			const auto log = m_logDatas.front();
			m_logDatas.pop();
			std::wstring result_str;

			switch (log.type)
			{
			case LOGTYPE::LOG_INFO:
				//time -> 시간 
				result_str += L"[LOG_INFO] ";
				break;

			case LOGTYPE::LOG_ERROR:
				result_str += L"[LOG_ERROR] ";
				break;

			case LOGTYPE::LOG_DEBUG:
				result_str += L"[LOG_DEBUG] ";
			}
			result_str += log.str;
			wprintf_s(L"%s", result_str.c_str());
		}
		std::this_thread::sleep_for(std::chrono::microseconds(20));
	}
}
