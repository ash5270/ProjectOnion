#pragma once
#include <chrono>
#include <ctime>

#include"SingleTon.h"
#include "../Util/Common.h"


using namespace std;
using namespace std::chrono;

#define DATETIME_FORMAT L"[%Y-%m-%d %H:%M:%S]"
#define DATE_FORMAT L"[%y-%m-%d]"
#define TIME_FORMAT L"[%H:%M:%S]"

#define STOP_WATCH_SIZE 10

namespace onion::util
{
	typedef enum
	{
		DAY_SUNDAY = 0,
		DAY_MONDAY =1,
		DAY_TUESDAY=2,
		DAY_WEDNESDAY=3,
		DAY_THURSDAY=4,
		DAY_FRIDAY=5,
		DAY_SATURDAY=6,
	}DayOfTheWeek;

	class Clock:public SingleTon<Clock>
	{
		time_t serverStartTime;
		wstring ToString(time_t time,wchar_t* fmt);
		//시작 시간 저장용
		time_point<system_clock>  m_vec_start_saves[STOP_WATCH_SIZE];
		bool m_vec_is_start[STOP_WATCH_SIZE];
	public:
		Clock();
		~Clock();

		time_t NowSystemTime();
		wstring ToString(time_t time, const wchar_t* str);
		wstring NowTime(const wchar_t* fmt);
		time_t GetServerStartTime();

		void StartChecking(int index);
		void EndChecking(int index);

	};

}

