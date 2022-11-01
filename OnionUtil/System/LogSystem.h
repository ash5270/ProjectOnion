///LogSystem
///로그 처리를 위한 클래스

#pragma once
#include "../Util/Common.h"

#include "../Util/SingleTon.h"
#include "LogData.h"

#include <mutex>
#include <queue>
#include<thread> 

namespace onion::system
{
	class LogSystem : public util::SingleTon<LogSystem>
	{
		bool m_isUpdate = false;

		std::mutex m_lock;
		std::queue<LogData> m_logDatas;
		std::thread m_thread;
			
	public:
		void Start();
		void Stop();
		void AddLog(LogData&& logData);

	private:
		void Update();
	};
}

