///Buffer Pool
///버퍼를 재사용하기 위한 클래스
///

#pragma once
#include <queue>
#include "safe_queue.h"
#include "Buffer.h"

namespace onion::system
{
	class BufferPool
	{
	private:
		int m_pool_size;
		int m_buf_size;
		safe_queue<Buffer*> m_usingPool;
		safe_queue<Buffer*> m_readyPool;
	public:
		BufferPool(int pool_size, int buffer_size = 1024) :
			m_pool_size(pool_size), m_usingPool(), m_readyPool(), m_buf_size(buffer_size)
		{
			for (int i = 0; i < pool_size; i++)
			{
				m_usingPool.push(new Buffer(buffer_size));
			}
		}
		~BufferPool()
		{
			for (int i = 0; i < m_readyPool.size(); i++)
			{
				Buffer* buffer;
				m_readyPool.try_Dequeue(buffer);
				delete buffer;
			}

			for (int i = 0; i < m_usingPool.size(); i++)
			{
				Buffer* buffer;
				m_usingPool.try_Dequeue(buffer);
				delete buffer;
			}
		}

		Buffer* GetBuffer()
		{
			Buffer* buffer;
			m_readyPool.try_Dequeue(buffer);
			m_usingPool.push(buffer);
			return buffer;
		}

		void Relese(Buffer* buffer)
		{
			
		}


	};

}
