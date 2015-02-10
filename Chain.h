#pragma once
#include "ArticulatedBody.h"

class Chain : public ArticulatedBody
{
public:
	Chain(vector3d* arr, int n);
	void render();
	GenMatrix<float, MAX_BONES * 3, 3>& Chain::getJacobian();
	void solverRotate(float* d0);
	~Chain();
};

