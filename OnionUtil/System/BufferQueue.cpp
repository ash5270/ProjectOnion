#include "BufferQueue.h"
#include <assert.h>
onion::system::BufferQueue::BufferQueue()
{
}

onion::system::BufferQueue::~BufferQueue()
{
}

size_t onion::system::BufferQueue::push_back(const Buffer* buffer)
{
	SpinLockGuard loc(lock);
	//std::lock_guard<std::mutex> lock(m_lock);
	queues.push(buffer);
	return queues.size();
}

const onion::system::Buffer* onion::system::BufferQueue::front()
{
	SpinLockGuard loc(lock);
	//std::lock_guard<std::mutex> lock(m_lock);
	if (queues.empty())
		return nullptr;
	assert(queues.front());
	const auto buf = queues.front();
	return buf;
}
