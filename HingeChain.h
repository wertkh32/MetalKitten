#pragma once
#include "ArticulatedBody.h"
class HingeChain :
	public ArticulatedBody
{
public:
	HingeChain(vector3d* arr,vector3d*axes, int n);
	void render();
	GenMatrix<float, MAX_BONES * 3, 3>& getJacobian(vector3d& goal);
	void solverRotate(float* d0);
	~HingeChain(void);
};

