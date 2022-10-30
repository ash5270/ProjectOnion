#pragma once
#include "Buffer.h"
#include <mutex>
#include <queue>
#include "SpinLock.h"
namespace onion::system
{
	class BufferQueue
	{
		std::queue<Buffer*> queues;
		std::mutex m_lock;

		SpinLock lock;

	public:
		BufferQueue(){}
		~BufferQueue(){}
	public:
		size_t push_back(Buffer* buffer)
		{
			SpinLockGuard loc(lock);
			//std::lock_guard<std::mutex> lock(m_lock);
			queues.emplace(buffer);
			return queues.size();
		}

		Buffer* front()
		{
			SpinLockGuard loc(lock);
			//std::lock_guard<std::mutex> lock(m_lock);
			if (queues.empty())
				return nullptr;
			const auto buf = queues.front();
			return buf;
		}

		void pop()
		{
			SpinLockGuard loc(lock);
			//std::lock_guard<std::mutex> lock(m_lock);
			if(queues.empty())
				return;
			queues.pop();
		}

		size_t size() 
		{
			//std::lock_guard<std::mutex> lock(m_lock);
			//SpinLockGuard loc(lock);
			return queues.size();
		}

		bool empty()
		{
			//std::lock_guard<std::mutex> lock(m_lock);
			//SpinLockGuard loc(lock);
			return queues.empty();
		}
	};
}

