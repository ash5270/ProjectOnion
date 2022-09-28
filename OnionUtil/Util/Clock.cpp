#include "Clock.h"

wstring onion::util::Clock::ToString(time_t time,const wchar_t* fmt) 
{
	wchar_t buf[128];
	//변환한 지역 시간을 설정할 구조체
	tm tm_;
	//초단위 시간으로 지역 일시를 구하는 함수
	localtime_s(&tm_, &time);
	//형식화된 날짜 및 시간으로 변환
	wcsftime(buf, 128, fmt, &tm_);
	return buf;
}

onion::util::Clock::Clock() : m_vec_start_saves(), m_vec_is_start()
{
	serverStartTime = this->NowSystemTime();
}

onion::util::Clock::~Clock()
{
	
}

time_t onion::util::Clock::NowSystemTime()
{
	return system_clock::to_time_t(system_clock::now());
}

wstring onion::util::Clock::NowTime(const wchar_t* fmt)
{
	return  this->ToString(this->NowSystemTime(), fmt);
}

time_t onion::util::Clock::GetServerStartTime()
{
	return serverStartTime;
}

void onion::util::Clock::StartChecking(int index)
{
	if(index < 0 || index > STOP_WATCH_SIZE)
		return;
	//true 면 현재 인덱스의 스탑워치 사용중
	if(m_vec_is_start[index])
	{
		PO_LOG(LOG_ERROR, L"%d is start..", index);
		return;
	}
	m_vec_is_start[index] = true;
	m_vec_start_saves[index] = system_clock::now();
}

void onion::util::Clock::EndChecking(int index)
{
	if (index < 0 || index > STOP_WATCH_SIZE)
		return;
	if (!m_vec_is_start[index])
	{
		PO_LOG(LOG_ERROR, L"%d is not start..", index);
		return;
	}
	m_vec_is_start[index] = false;
	auto end = system_clock::now();

	duration<double> elapsed = end - m_vec_start_saves[index];
	PO_LOG(LOG_INFO, L"[%d] stop watch time is :[%f]", index, elapsed.count());
}
