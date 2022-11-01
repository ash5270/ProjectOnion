///safe list
///멀티쓰레드를 위한 list
#pragma once

#include <list>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace onion::system
{
	template<class T>
	class safe_list
	{
	private:
		std::list<T> m_list;
		std::mutex m_lock;
	public:
		void push_back(T data)
		{
			std::lock_guard<std::mutex> lock(m_lock);
			m_list.push_back(data);
		}

		void push_front(T data)
		{
			std::lock_guard<std::mutex> lock(m_lock);
			m_list.push_front(data);
		}
	};
}

