#pragma once

namespace onion::object::math
{
	class Vector2;
	class Vector3
	{
	public:
		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const Vector3& vector3);
		Vector3(const Vector2& vector2);



		void operator+=(const Vector3& vector3);
		void operator-=(const Vector3& vector3);

		void operator+=(const Vector2& vector3);
		void operator-=(const Vector2& vector3);

		Vector3 operator+(const Vector3& vector3);
		Vector3 operator-(const Vector3& vector3);

		Vector3 operator+(const Vector2& vector3);
		Vector3 operator-(const Vector2& vector3);


		static float Distance(const Vector3& a, const Vector3& b);

		float x;
		float y;
		float z;
	};
}