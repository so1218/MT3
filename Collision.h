#pragma once
#include "Structures.h"

bool IsCollision(const Sphere& sphere, const Plane& plane);
bool IsCollision(const Segment& segment, const Plane& plane);
bool IsCollision(const Triangle& triangle, const Segment& segment);
bool IsCollision(const AABB& aabb1, const AABB& aabb2);
bool IsCollision(const AABB& aabb, const Segment& segment);
bool IsCollision(const AABB& aabb, const Sphere& sphere);
bool IsCollision(const Sphere& s1, const Sphere& s2);
bool IsCollision(const Sphere& ball, const Capsule& capsule);