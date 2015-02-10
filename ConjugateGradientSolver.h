#pragma once

#include "includes.h"
#include "ConstrainedRows.h"

#define MAX_ITER 20
#define EPSILON 0.01


//use pure arrays
class ConjugateGradientSolver
{
	float *r, *x, *d, *q, *tempo;
	float *flatA;
	float** A;
	int n;

public:
	ConjugateGradientSolver();
	~ConjugateGradientSolver(void);
	void initSolver(int _n, float** A);

	void solve(float* x, float* b);
	void solveWithConstraints(float* x, float* b, bool* allowed);

	float dot(float* a, float* b, int k)
	{
		float r=0;
		for(int i=0;i<k;i++)
			r+=a[i]*b[i];
		return r;
	}
	
	void removeRows(int r);
};

