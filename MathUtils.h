#pragma once
#include "Structures.h"

// 正射影ベクトルを求める関数
Vector3 Project(const Vector3& v1, const Vector3& v2);
// 最近接点を求める関数
Vector3 CrosestPoint(const Vector3& point, const Segment& segment);
// クロス積を求める関数
Vector3 Cross(const Vector3& v1, const Vector3& v2);
float DistanceSquared(const Vector3& a, const Vector3& b);
Vector3 Perpendicular(const Vector3& vector);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
float Length(const Vector3& v);
Vector3 Reflect(const Vector3& input, const Vector3& normal);
Vector3 ClosestPointOnSegment(const Vector3& p, const Segment& seg);