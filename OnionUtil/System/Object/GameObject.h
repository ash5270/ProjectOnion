#pragma once
#include "Object.h"

namespace onion::system::object
{
	class GameObjcet : public  NameObject , public Work
	{
		float m_direction;
	public:
		virtual ~GameObjcet()
		{
			this->Free();
		}
		virtual void Initialize(){}
		virtual void Free(){}
	};
}