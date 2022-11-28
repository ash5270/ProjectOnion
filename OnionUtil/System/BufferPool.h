///Buffer Pool
///버퍼를 재사용하기 위한 클래스
///

#pragma once
#include <queue>
#include "safe_queue.h"
#include "Buffer.h"

#include "../Util/SingleTon.h"

namespace onion::system
{
	class BufferPool : public util::SingleTon<BufferPool>
	{
	private:
		int m_pool_size;
		int m_buf_size;
		safe_queue<Buffer*> m_readyPool;

	public:
		void Init(int pool_size,int buffer_size =512)
		{
			m_pool_size = pool_size;
			m_buf_size = buffer_size;

			InitPool();
			PO_LOG(LOG_INFO, L"Buffer pool init sucess...\n");
		}

		void Delete()
		{
			for (int i = 0; i < m_readyPool.size(); i++)
			{
				Buffer* buffer;
				m_readyPool.try_Dequeue(buffer);
				delete buffer;
			}
		}

		Buffer* GetBuffer()
		{
			Buffer* buffer;
			if (m_readyPool.try_Dequeue(buffer))
				return buffer;
			else
			{
				return nullptr;
			}
		}

		void Relese(Buffer* buffer)
		{
			buffer->Clear();
			m_readyPool.push(buffer); 
		}

	private:

		void InitPool()
		{
			for(int i=0; i< m_pool_size;i++)
			{
				m_readyPool.push(new Buffer(m_buf_size));
			}
		}
	};

}
