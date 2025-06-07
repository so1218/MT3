#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

struct Vector2
{
	float x, y;
};

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator*(float scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	Vector3 Normalize() const
	{
		float length = std::sqrtf(x * x + y * y + z * z);
		if (length == 0.0f)
		{
			return { 0,0,0 };
		}
		return { x / length,y / length,z / length };
	}

	static float Dot(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

};

inline float Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3 Normalize(const Vector3& vector)
{
	float length = std::sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	if (length == 0.0f)
	{
		return { 0,0,0 };
	}
	return { vector.x / length, vector.y / length, vector.z / length };
}

inline Vector3 Multiply(float scalar, const Vector3& vector)
{
	return
	{
		scalar * vector.x,
		scalar * vector.y,
		scalar * vector.z
	};
}

inline Vector3 Multiply(const Vector3& a, const Vector3& b)
{
	return
	{
		a.x * b.x,
		a.y * b.y,
		a.z * b.z
	};
}

struct Sphere
{
	Vector3 center;
	float radius;
	uint32_t color;
};

struct Line
{
	Vector3 origin;// 始点
	Vector3 diff;// 終点への差分ベクトル
};

struct Ray
{
	Vector3 origin;// 始点
	Vector3 diff;// 終点への差分ベクトル
};

struct Segment
{
	Vector3 origin;// 始点
	Vector3 diff;// 終点への差分ベクトル
	uint32_t color;
};

struct Plane
{
	Vector3 normal;// 法線
	float distance;// 距離
	uint32_t color;
};

struct Triangle
{
	Vector3 vertices[3];
};

struct AABB
{
	Vector3 min;
	Vector3 max;
	uint32_t color;
};