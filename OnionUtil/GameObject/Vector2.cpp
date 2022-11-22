#include "Vector2.h"
//#include <cmath>

onion::object::math::Vector2::Vector2()
{
	x = y = 0;
}

onion::object::math::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

onion::object::math::Vector2::Vector2(const Vector2& vector2)
{
	this->x = vector2.x;
	this->y = vector2.y;
}

onion::object::math::Vector2::Vector2(const Vector3& vector3)
{
	this->x = vector3.x;
	this->y = vector3.y;
}

void onion::object::math::Vector2::operator+=(const Vector2& vector2)
{
	this->x += vector2.x;
	this->y += vector2.y;
}

void onion::object::math::Vector2::operator-=(const Vector2& vector2)
{
	this->x -= vector2.x;
	this->y -= vector2.y;
}

onion::object::math::Vector2 onion::object::math::Vector2::operator+(const Vector2& vector2)
{
	Vector2 new_vec = Vector2(x + vector2.x, y + vector2.y);
	return  new_vec;
}

onion::object::math::Vector2 onion::object::math::Vector2::operator-(const Vector2& vector2)
{
	Vector2 new_vec = Vector2(x - vector2.x, y - vector2.y);
	return  new_vec;
}

float onion::object::math::Vector2::Distance(const Vector2& a, const Vector2& b)
{
	/*float dis = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));
	return sqrt(dis);*/
	return 0;
}
