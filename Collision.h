#pragma once

#include "Matrix3d.h"
#include "Geometry.h"


class Collision
{
public:
	static bool testLineCCWTriangle(Line& l, Triangle& t, vector3d* out);
	static bool testLineTriangle(Line& l, Triangle& t, vector3d* out);
	static bool testTriangleTriangle(Triangle& t1, Triangle& t2, vector3d* out);
	static bool testAABBAABB(AABB& a, AABB& b);
};

