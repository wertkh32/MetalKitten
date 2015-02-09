#pragma once
#include "bone.h"
class HingeJointBone :
	public bone
{
	vector3d axis;
	float angle;
public:
	void rotate(float angle);

	HingeJointBone(void);
	~HingeJointBone(void);
};

