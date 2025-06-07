#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "MathUtils.h"

// 正射影ベクトルを求める関数
Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	float dot = Vector3::Dot(v1, v2); // v1とv2の内積
	float v2LengthSquared = Vector3::Dot(v2, v2); // v2の長さの2乗
	float scalar = dot / v2LengthSquared; // スカラー係数
	return { v2.x * scalar, v2.y * scalar, v2.z * scalar }; // 正射影ベクトル
}

// 最近接点を求める関数
Vector3 CrosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 segmentVec = { segment.diff.x - segment.origin.x, segment.diff.y - segment.origin.y, segment.diff.z - segment.origin.z };
	Vector3 startToPoint = { point.x - segment.origin.x, point.y - segment.origin.y ,point.z - segment.origin.z };

	float t = Vector3::Dot(startToPoint, segmentVec) / Vector3::Dot(segmentVec, segmentVec);

	// 0.0fから1.0f
	t = std::clamp(t, 0.0f, 1.0f);

	return { segment.origin.x + segmentVec.x * t, segment.origin.y + segmentVec.y * t, segment.origin.z + segmentVec.z * t };
}


// クロス積を求める関数
Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

float DistanceSquared(const Vector3& a, const Vector3& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return dx * dx + dy * dy + dz * dz;
}



Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return{ -vector.y,vector.x,0.0f };
	}
	return{ 0.0f,-vector.z, vector.y };
}


Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	// v1 と v2 の各成分を引き算して、新しいベクトルを返す
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	// v1 と v2 の各成分を足し算して、新しいベクトルを返す
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return 
	{
		v1.x + (v2.x - v1.x) * t,
		v1.y + (v2.y - v1.y) * t,
		v1.z + (v2.z - v1.z) * t
	};
}

float Length(const Vector3& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Reflect(const Vector3& input, const Vector3& normal)
{
	float dot = Vector3::Dot(input, normal);
	return {
		input.x - 2.0f * dot * normal.x,
		input.y - 2.0f * dot * normal.y,
		input.z - 2.0f * dot * normal.z
	};
}

Vector3 ClosestPointOnSegment(const Vector3& p, const Segment& seg)
{
	Vector3 a = seg.origin;
	Vector3 b = seg.origin + seg.diff;
	Vector3 ab = b - a;
	float t = Vector3::Dot(p - a, ab) / Vector3::Dot(ab, ab);
	t = std::clamp(t, 0.0f, 1.0f);
	return a + ab * t;
}