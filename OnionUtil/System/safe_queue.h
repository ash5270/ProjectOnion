///Mutex Queue
///멀티쓰레드를 위한 Queue
///시간 남으면 stl queue가 아닌 직접 구현도 재미있을듯
#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace onion::system
{
	template<class  T>
	class safe_queue
	{
	private:
		std::mutex m_lock;
		std::condition_variable m_condition;
		std::queue<T> m_queue;

	public:
		safe_queue() : m_lock(),m_condition(), m_queue()
		{}
		~safe_queue(){}

		void push(T data)
		{
			std::unique_lock<std::mutex> lock(m_lock);
			m_queue.push(std::move(data));
			m_condition.notify_one();
		}

		bool try_Dequeue(T& data,std::chrono::milliseconds timer= std::chrono::milliseconds(10))
		{
			std::unique_lock<std::mutex> lock(m_lock);
			if (!m_condition.wait_for(lock, timer, [&] {return !m_queue.empty(); }))
				return false;

			data = std::move(m_queue.front());
			m_queue.pop();
			return true;
		}

		size_t size()
		{
			return m_queue.size();
		}
	};
}
