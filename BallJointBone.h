#pragma once
#include "bone.h"
class BallJointBone :
	public bone
{
	vector3d angles;
public:
	void rotate(vector3d angles);
	
	BallJointBone(void);
	~BallJointBone(void);
};

