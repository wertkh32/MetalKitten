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
		float w = CLAMP(q.s,-0.999999,0.999999);
		float angle = (acos(w) * 2.0) * ccdaxis.dot(rotaxis);
		return quatn(rotaxis, angle);
	};
public:
	void rotate(quatn& p);
	void rotate(float angle);
	vector3d getJacobianRow(vector3d& endeffector);
	void render();

	HingeJointBone(vector3d& _pos, vector3d& _endpos, vector3d& _axis) : bone(_pos, _endpos), axis(_axis){};
	~HingeJointBone(void);
};

