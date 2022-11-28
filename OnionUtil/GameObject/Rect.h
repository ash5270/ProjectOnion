#pragma once
#include "Vector2.h"

namespace onion::object::math
{
	struct Rect
	{
		Vector2 leftTop = {};
		Vector2 rightBtm = {};

		bool BoundRect(const math::Vector2& position);
		bool BoundRect(const Rect& rect);
		void VecToRec(const math::Vector2& pos, int size);
	};
}

