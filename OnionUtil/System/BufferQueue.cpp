#include "BufferQueue.h"
#include <assert.h>
onion::system::BufferQueue::BufferQueue()
{
}

onion::system::BufferQueue::~BufferQueue()
{
}

size_t onion::system::BufferQueue::push_back(Buffer* buffer)
{
	//SpinLockGuard loc(lock);
	std::lock_guard<std::mutex> lock(m_lock);
	queues.push(buffer);
	return queues.size();
}

 onion::system::Buffer* onion::system::BufferQueue::front()
{	
	//SpinLockGuard loc(lock);
	std::lock_guard<std::mutex> lock(m_lock);
	if (queues.empty())
		return nullptr;
	const auto buf = queues.front();
	return buf;
}
