#pragma once
#include "includes.h"

enum GEO_TYPE
{
	_POINT = 0,
	_LINE = 1,
	_TRIANGLE = 2,
	_CUBE = 3,
	_SPHERE = 4,
	_AABB = 5
};

struct Geometry
{
	int colliderIndex;
	GEO_TYPE type;
	Geometry(){};
	~Geometry(){};
};

struct Point : public Geometry
{
	vector3d &p;
	Point(vector3d& _p) :p(_p)
	{
		type = _POINT;
	}
	~Point(){};
};

struct Line : public Geometry
{
	vector3d &p0, &p1;
	Line(vector3d& _p0, vector3d& _p1) :p0(_p0), p1(_p1)
	{
		type = _LINE;
	}
	~Line(){}
};

struct Triangle : public Geometry
{
	vector3d &p0, &p1, &p2;
	
	Triangle(vector3d& _p0, vector3d& _p1, vector3d& _p2) :p0(_p0), p1(_p1), p2(_p2)
	{
		type = _TRIANGLE;
	}
	
	~Triangle(){}
};

struct AABB : public Geometry
{
	vector3d min, max;
	AABB(vector3d& _min, vector3d& _max) :min(_min), max(_max)
	{
		type = _AABB;
	}
	AABB(){}

	bool testAABB(AABB& aabb)
	{
		if (max.x < aabb.min.x || min.x > aabb.max.x) return false;
		if (max.y < aabb.min.y || min.y > aabb.max.y) return false;
		if (max.z < aabb.min.z || min.z > aabb.max.z) return false;
		return true;
	}

	~AABB(){};
};


