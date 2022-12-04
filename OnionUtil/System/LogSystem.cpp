#include "LogSystem.h"
#include "../Util/Clock.h"
void onion::system::LogSystem::Start()
{
	m_isUpdate = true;
	m_thread = std::thread(&LogSystem::Update, this);
	//error
	setlocale(LC_ALL, "");
	std::wcout.imbue(std::locale(""));
	PO_LOG(LOG_INFO, L"LogSystem Start...\n");
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
	while (m_isUpdate||m_logDatas.size()>0)
	{
		if (!m_logDatas.empty())
		{
			//lock
			std::lock_guard<std::mutex> lock(m_lock);
			
			std::wstring result_str;
			result_str += util::Clock::getInstance().NowTime(DATETIME_FORMAT);
			switch (m_logDatas.front().type)
			{
			case LOGTYPE::LOG_INFO:
				result_str += L"[LOG_INFO] ";
				break;

			case LOGTYPE::LOG_ERROR:
				result_str += L"[LOG_ERROR] ";
				break;

			case LOGTYPE::LOG_DEBUG:
				result_str += L"[LOG_DEBUG] ";
				break;
			}
			result_str += m_logDatas.front().str;
			wprintf_s(L"%s", result_str.c_str());
			m_logDatas.pop();
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1));
 	}
}


