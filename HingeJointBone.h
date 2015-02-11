#pragma once
#include "bone.h"
class HingeJointBone :
	public bone
{
	vector3d axis;
	quatn applyConstraints(quatn& q)
	{
		vector3d& ccdaxis = q.v.unit();
		vector3d rotaxis = rotation * axis;
		float angle = (acos(q.s) * 2.0) * ccdaxis.dot(rotaxis);
		return quatn(rotaxis, angle);
	};
public:
	void rotate(float angle);
	void projectedRotate(vector3d& ccdaxis, float angle);
	vector3d getJacobianRow(vector3d& endeffector);
	void render();

	HingeJointBone(vector3d& _pos, vector3d& _endpos, vector3d& _axis) : bone(_pos, _endpos), axis(_axis){};
	~HingeJointBone(void);
};

