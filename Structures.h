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

	Vector3& operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

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

	Vector3 operator/(float scalar) const
	{
		return scalar != 0 ? Vector3(x / scalar, y / scalar, z / scalar) : Vector3(0, 0, 0);
	}
	// クロス積 (外積)
	Vector3 cross(const Vector3& other) const {
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	// ベクトルの長さ (マグニチュード)
	float length() const {
		return std::sqrt(x * x + y * y + z * z);
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

inline Vector3 operator*(float scalar, const Vector3& vec) {
	return Vector3{ vec.x * scalar, vec.y * scalar, vec.z * scalar };
}

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

struct Spring
{
	// アンカー。固定された端の位置
	Vector3 anchor;
	float naturalLength; // 自然長
	float stiffness; // ばね定数。剛性
	float dampingCoefficient; // 減衰係数
};

struct Ball
{
	Vector3 position; // 位置
	Vector3 velocity; // 速度
	Vector3 acceleration; // 加速度
	float mass; // 質量
	float radius; // 半径
	unsigned int color; // 色
};

struct Pendulum
{
	Vector3 anchor; // アンカーポイント。固定された端の位置
	float length; // 紐の長さ
	float angle; // 現在の角度
	float angularVelocity; // 角速度ω
	float angularAcceleration; // 角加速度
};

struct ConicalPendulum
{
	Vector3 anchor; // アンカーポイント。固定された端の位置
	float length; // 紐の長さ
	float halfApexAngle; // 円錐の頂角の半分
	float angle; // 現在の角度
	float angularVelocity; // 角速度ω
};

struct Capsule
{
	Segment segment;
	float radius;
};