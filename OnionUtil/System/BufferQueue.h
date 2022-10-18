#pragma once
#include "Buffer.h"
#include <mutex>
#include <queue>
#include "SpinLock.h"
namespace onion::system
{

	class BufferQueue
	{
		std::queue<const Buffer*> queues;
		std::mutex m_lock;

		SpinLock lock;
	public:
		BufferQueue();
		~BufferQueue();
	public:
		size_t push_back(const Buffer* buffer);
		const Buffer* front();
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
			SpinLockGuard loc(lock);
			return queues.size();
		}

		bool empty()
		{
			//std::lock_guard<std::mutex> lock(m_lock);
			SpinLockGuard loc(lock);
			return queues.empty();
		}
	};
}
