#pragma once

#include "Matrix3d.h"
#include "Geometry.h"

struct CollisionInfo
{
	QuickArray<vector3d, 4> hits;
	vector3d norm;
};


class Collision
{
	static vector3d closestPtPointTriangle(vector3d& p, Triangle& t);
public:
	static bool testLineCCWTriangle(Line& l, Triangle& t, vector3d* out);
	static bool testLineTriangle(Line& l, Triangle& t, vector3d* out);
	static bool testTriangleTriangle(Triangle& t1, Triangle& t2, vector3d* out);
	static bool testAABBAABB(AABB& a, AABB& b);
	static bool testTriangleSphere(Triangle& t, Sphere& s, CollisionInfo* info);
};

