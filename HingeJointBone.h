#pragma once
#include "bone.h"
class HingeJointBone :
	public bone
{
	vector3d axis;
	float angle;
public:
	void rotate(float angle);
	vector3d getJacobianRow(vector3d& endeffector);
	void render();

	HingeJointBone(vector3d& _pos, vector3d& _endpos, vector3d& _axis) : bone(_pos, _endpos), axis(_axis){};
	~HingeJointBone(void);
};

