#include <algorithm>

#include "Collision.h"
#include "MathUtils.h"

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	// 球の中心と平面の距離を計算
	float distanceFromCenterToPlane = std::abs(
		Dot(plane.normal, sphere.center) - plane.distance
	);

	// 球の半径以下なら衝突とみなす
	return distanceFromCenterToPlane <= sphere.radius;
}

bool IsCollision(const Segment& segment, const Plane& plane)
{
	// 線の方向ベクトルと平面の法線ベクトルの内積を計算
	float dot = Dot(plane.normal, segment.diff);

	// 線が平面と平行な場合
	if (dot == 0.0f)
	{
		return false;
	}

	// 交点までのパラメーター t を計算
	float t = (plane.distance - Dot(plane.normal, segment.origin)) / dot;

	// 交点が線分上にあるかチェック
	return t >= 0.0f && t <= 1.0f;
}

bool IsCollision(const Triangle& triangle, const Segment& segment)
{
	// 線分と三角形を含む平面の交差判定
	// 三角形の法線を計算
	Vector3 edge1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 edge2 = Subtract(triangle.vertices[2], triangle.vertices[0]);
	Vector3 triangleNormal = Normalize(Cross(edge1, edge2));

	// 三角形を含む平面を定義
	Plane trianglePlane;
	trianglePlane.normal = triangleNormal;
	trianglePlane.distance = Dot(triangleNormal, triangle.vertices[0]); // 平面までの距離

	// 線分が平面と平行な場合
	float dotSegmentNormal = Dot(segment.diff, trianglePlane.normal);
	if (dotSegmentNormal == 0.0f) {
		return false;
	}

	// 交点までのパラメーター t を計算
	float t = (trianglePlane.distance - Dot(trianglePlane.normal, segment.origin)) / dotSegmentNormal;

	// 交点が線分上にあるかチェック
	if (t < 0.0f || t > 1.0f) {
		return false; // 線分の外で交差している
	}

	// 交点を計算
	Vector3 intersectionPoint = Add(segment.origin, Multiply(t, segment.diff));

	// 2交点が三角形の内部にあるかの判定

	// V0を始点とする辺V0V1、V0から交点Pへのベクトル
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v0p = Subtract(intersectionPoint, triangle.vertices[0]);
	Vector3 cross01 = Cross(v01, v0p);

	// V1を始点とする辺V1V2、V1から交点Pへのベクトル
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v1p = Subtract(intersectionPoint, triangle.vertices[1]);
	Vector3 cross12 = Cross(v12, v1p);

	// V2を始点とする辺V2V0、V2から交点Pへのベクトル
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
	Vector3 v2p = Subtract(intersectionPoint, triangle.vertices[2]);
	Vector3 cross20 = Cross(v20, v2p);

	// 各クロス積と三角形の法線との内積を計算
	float dot01 = Dot(cross01, triangleNormal);
	float dot12 = Dot(cross12, triangleNormal);
	float dot20 = Dot(cross20, triangleNormal);

	// すべての内積が同じ符号（または0）であれば、交点は三角形の内部にある

	return (dot01 == 0 && dot12 == 0 && dot20 == 0) ||
		(dot01 == 0 && dot12 == 0 && dot20 == 0);
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z))
	{
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Segment& segment)
{
	float entryTime = 0.0f;
	float exitTime = 1.0f;

	Vector3 segmentEnd = Add(segment.origin, segment.diff);
	Vector3 direction =
	{
		segmentEnd.x - segment.origin.x,
		segmentEnd.y - segment.origin.y,
		segmentEnd.z - segment.origin.z
	};

	for (int axis = 0; axis < 3; ++axis) {
		float segmentStart = 0.0f;
		float segmentDir = 0.0f;
		float boxMin = 0.0f;
		float boxMax = 0.0f;

		// X軸
		if (axis == 0)
		{
			segmentStart = segment.origin.x;
			segmentDir = direction.x;
			boxMin = aabb.min.x;
			boxMax = aabb.max.x;
		}
		// Y軸
		else if (axis == 1)
		{
			segmentStart = segment.origin.y;
			segmentDir = direction.y;
			boxMin = aabb.min.y;
			boxMax = aabb.max.y;
		}
		// Z軸
		else
		{
			segmentStart = segment.origin.z;
			segmentDir = direction.z;
			boxMin = aabb.min.z;
			boxMax = aabb.max.z;
		}

		if (std::abs(segmentDir) < 1e-11f)
		{
			if (segmentStart < boxMin || segmentStart > boxMax)
			{
				return false;
			}
		}
		else
		{
			float t1 = (boxMin - segmentStart) / segmentDir;
			float t2 = (boxMax - segmentStart) / segmentDir;

			if (t1 > t2)
			{
				std::swap(t1, t2);
			}
			if (t1 > entryTime)
			{
				entryTime = t1;
			}

			if (t2 < exitTime)
			{
				exitTime = t2;
			}
			if (entryTime > exitTime || exitTime < 0.0f)
			{
				return false;
			}
		}
	}

	return entryTime <= 1.0f;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
	// 球の中心をAABBに投影(最近傍点を求める)
	Vector3 closestPoint;
	closestPoint.x = std::clamp(sphere.center.x, aabb.min.x, aabb.max.x);
	closestPoint.y = std::clamp(sphere.center.y, aabb.min.y, aabb.max.y);
	closestPoint.z = std::clamp(sphere.center.z, aabb.min.z, aabb.max.z);

	// 最近傍点との距離の2乗を計算し、半径の2乗以下なら衝突
	float distSq = DistanceSquared(closestPoint, sphere.center);
	return distSq <= sphere.radius * sphere.radius;
}

bool IsCollision(const Sphere& s1, const Sphere& s2)
{
	float dx = s2.center.x - s1.center.x;
	float dy = s2.center.y - s1.center.y;
	float dz = s2.center.z - s1.center.z;

	float radiusSum = s1.radius + s2.radius;

	return dx * dx + dy * dy + dz * dz <= radiusSum * radiusSum;
}

bool IsCollision(const Sphere& ball, const Capsule& capsule)
{
	Vector3 closest = ClosestPointOnSegment(ball.center, capsule.segment);
	float distance = Length(ball.center - closest);
	return distance <= (ball.radius + capsule.radius);
}
