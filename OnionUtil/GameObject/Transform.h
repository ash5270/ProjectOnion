#pragma once
#include "OnionMath.h"
namespace onion::object
{
	class Transform
	{
	public:
		Transform();
		~Transform();

		math::Vector3 position;
		math::Vector2 velocity;
	};
}