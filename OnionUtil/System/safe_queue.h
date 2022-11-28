///Mutex Queue
///멀티쓰레드를 위한 Queue
///시간 남으면 stl queue가 아닌 직접 구현도 재미있을듯
#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "SpinLock.h"
#include <chrono>

namespace onion::system
{
	template<class  T>
	class safe_queue
	{
	private:
		std::mutex m_lock;
		SpinLock m_spin_lock;
		std::queue<T> m_queue;
	public:
		safe_queue() : m_lock(), m_queue()
		{}
		~safe_queue(){}

		void push(T data)
		{
			std::lock_guard<std::mutex> lock(m_lock);
			//SpinLockGuard locking(m_spin_lock);
			m_queue.push(std::move(data));
		}

		bool try_Dequeue(T& data)
		{
			std::lock_guard<std::mutex> lock(m_lock);
			//SpinLockGuard locking(m_spin_lock);
			data = std::move(m_queue.front());
			if (data==nullptr)
				return false;
			m_queue.pop();
			return true;
		}
			
		size_t size()
		{
			std::lock_guard<std::mutex> lock(m_lock);
			//SpinLockGuard locking(m_spin_lock);
			return m_queue.size();
		}

		bool empty()
		{
			//SpinLockGuard locking(m_spin_lock);
			return m_queue.size() == 0 ? true : false;
		}
	};
}