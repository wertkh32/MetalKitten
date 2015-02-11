#pragma once
#include "bone.h"
class BallJointBone :
	public bone
{
public:
	void rotate(quatn& q);
	void rotate(vector3d& angles);
	Matrix3d getJacobian3DOF(vector3d& endeffector);
	vector3d getJacobianQuatn(vector3d& endeffector, vector3d& goal, vector3d& out_axis);
	void render();

	BallJointBone(vector3d& _pos, vector3d& _endpos) :bone(_pos, _endpos){}
	~BallJointBone(void);
};

