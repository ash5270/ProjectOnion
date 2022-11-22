#pragma once
#include "Object.h"

#include "Transform.h"

namespace onion::object
{
	class GameObject :public Object
	{
	public:
		GameObject();
		virtual ~GameObject();

		Transform transform;
	};
}
