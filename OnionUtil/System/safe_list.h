///safe list
///멀티쓰레드를 위한 list
/// 
#pragma once

#include <list>
#include <mutex>
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
		void push_back(const T& data)
		{
			std::lock_guard<std::mutex> lock(m_lock);
			m_list.push_back(data);
		}

		void push_front(const T& data)
		{
			std::lock_guard<std::mutex> lock(m_lock);
			m_list.push_front(data);
		}

		T pop_back()
		{
			std::lock_guard<std::mutex> lock(m_lock);
			if (m_list.empty())
				return NULL;
			T data = m_list.back();
			m_list.pop_back();
			return std::move(data);
		}

		bool find(const T& data)
		{
			std::lock_guard<std::mutex> lock(m_lock);
		    auto it = std::find(m_list.begin(), m_list.end());
			if (it != m_list.end())
				return true;
			else
				return false;
		}

		bool remove(const T& data)
		{
			std::lock_guard<std::mutex> lock(m_lock);
			auto it = m_list.remove(data);
			if (it != m_list.begin())
				return true;
			else
				return false;
		}

		size_t size()
		{
			return m_list.size();
		}

		bool empty()
		{
			return m_list.empty();
		}
	};
}

