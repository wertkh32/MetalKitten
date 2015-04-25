#pragma once

#include "includes.h"
#include "ConstrainedRows.h"
#include "SparseMatrix.h"

#define MAX_ITER 100
#define EPSILON 0.01


//use pure arrays
class ConjugateGradientSolver
{
	float *r, *x, *d, *q, *tempo;
	float *flatA;
	float** A;
	int n;
	SparseMatrix* SA;

public:
	ConjugateGradientSolver();
	~ConjugateGradientSolver(void);
	void initSolver(int _n, float** __restrict A);
	void initSparseSolverCompressed3x3(int _n, float** __restrict A);

	void solve(float* __restrict x, float* __restrict b);
	void solveWithConstraints(float* __restrict x, float* __restrict b, bool* __restrict allowed);
	void solveSparseCompressed3x3(float* __restrict x, float* __restrict b);


	float dot(float* __restrict a, float* __restrict b, int k)
	{
		float r=0;
		for(int i=0;i<k;i++)
			r+=a[i]*b[i];
		return r;
	}
	
	void removeRows(int r);
};

