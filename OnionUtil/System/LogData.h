#pragma once
#include <string>

typedef enum
{
	LOG_INFO =0,
	LOG_ERROR=1,
	LOG_DEBUG=2,
}LOGTYPE;

namespace onion::system
{
	struct LogData
	{
		LogData(LOGTYPE type,std::wstring&& log)
		{
			this->type = type;
			this->str = std::move(log);
		}
		LOGTYPE type;
		std::wstring str;
	};

}

