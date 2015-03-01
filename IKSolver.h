#pragma once
#include "ArticulatedBody.h"
#include "ConjugateGradientSolver.h"
#define EPSILON 0.01
#define MAX_ITERATIONS 100

class IKSolver
{
	int n;
	float** JTJ;
	float*  JTs;
	float*  d0;
	ArticulatedBody& artbody;
	ConjugateGradientSolver solver;
public:
	IKSolver(ArticulatedBody& _artbody);
	void solveByJacobianInverse(vector3d& goal);
	void solveByJacobianRightInverse(vector3d& goal);
	void solveByCCD(vector3d& goal);

	~IKSolver();
};

