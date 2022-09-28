#pragma once

#include <atomic>

class SpinLock
{
	std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
	void lock()
	{
		while (locked.test_and_set(std::memory_order_acquire));
	}

	bool try_lock()
	{
		return !locked.test_and_set(std::memory_order_acquire);
	}

	void unlock()
	{
		locked.clear(std::memory_order_release);
	}

	void clear()
	{
		unlock();
	}
};

class SpinLockGuard
{
	SpinLock& m_lock;
public:
	SpinLockGuard(SpinLock& lock) : m_lock(lock)
	{
		m_lock.lock();
	}

	~SpinLockGuard()
	{
		m_lock.unlock();
	}
};