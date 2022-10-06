#pragma once

#include "LogSystem.h"
#include "LogData.h"

//수정해야할 곳 
#define PO_LOG(log_level,format,...)\
	{\
	auto log_str = wstring_format(format,##__VA_ARGS__);\
	onion::system::LogData log_data(log_level,std::move(log_str));\
	onion::system::LogSystem::getInstance().AddLog(std::move(log_data));\
	}

