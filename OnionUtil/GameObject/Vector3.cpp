#include "Vector3.h"
#include "Vector2.h"
//#include <cmath>
onion::object::math::Vector3::Vector3()
{
	x = y = z = 0;
}

onion::object::math::Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

onion::object::math::Vector3::Vector3(const Vector3& vector3)
{
	this->x = vector3.x;
	this->y = vector3.y;
	this->z = vector3.z;
}

onion::object::math::Vector3::Vector3(const Vector2& vector2)
{
	this->x = vector2.x;
	this->y = vector2.y;
	this->z = 0;
}

void onion::object::math::Vector3::operator+=(const Vector3& vector3)
{
	this->x += vector3.x;
	this->y += vector3.y;
	this->z += vector3.z;
}

void onion::object::math::Vector3::operator-=(const Vector3& vector3)
{
	this->x -= vector3.x;
	this->y -= vector3.y;
	this->z -= vector3.z;
}

void onion::object::math::Vector3::operator+=(const Vector2& vector3)
{
	this->x += vector3.x;
	this->y += vector3.y;
}

void onion::object::math::Vector3::operator-=(const Vector2& vector3)
{
	this->x -= vector3.x;
	this->y -= vector3.y;
}

onion::object::math::Vector3 onion::object::math::Vector3::operator+(const Vector2& vector3)
{
	Vector3 new_vec = Vector3(x + vector3.x, y + vector3.y, z);
	return new_vec;
}

onion::object::math::Vector3 onion::object::math::Vector3::operator-(const Vector2& vector3)
{
	Vector3 new_vec = Vector3(x + vector3.x, y + vector3.y, z);
	return new_vec;
}



onion::object::math::Vector3 onion::object::math::Vector3::operator+(const Vector3& vector3)
{
	Vector3 new_vec = Vector3(x + vector3.x, y + vector3.y, z + vector3.z);
	return new_vec;
}

onion::object::math::Vector3 onion::object::math::Vector3::operator-(const Vector3& vector3)
{
	Vector3 new_vec = Vector3(x - vector3.x, y - vector3.y, z - vector3.z);
	return new_vec;
}

float onion::object::math::Vector3::Distance(const Vector3& a, const Vector3& b)
{
	/*float dis = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z));
	return sqrt(dis);*/
	return 0;
}
