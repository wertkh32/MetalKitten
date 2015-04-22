#pragma once
#include "includes.h"

struct Node
{
	vector3d position;
	float mass;
	Node(){};
	Node(vector3d& _position, float _mass) : position(_position), mass(_mass){};
	~Node(){};
};

