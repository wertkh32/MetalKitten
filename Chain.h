#pragma once
#include "ArticulatedBody.h"

#define USE_QUATN_JACOBIAN

class Chain : public ArticulatedBody
{
	#ifdef USE_QUATN_JACOBIAN
		QuickArray<vector3d,MAX_BONES> quatn_axes;
	#endif

public:
	Chain(vector3d* arr, int n);
	void render();
	GenMatrix<float, MAX_BONES * 3, 3>& Chain::getJacobian(vector3d& goal);
	void solverRotate(float* d0);
	~Chain();

};

