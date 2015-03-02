#pragma once
#include "includes.h"

class MatrixOps
{
public:
	MatrixOps();

	static void CholeskyFac(float** __restrict in_A, float** __restrict out_LT, int dim);
	static void InverseMatrix(float** __restrict in_A, float** __restrict out_Ainv, int dim);

	~MatrixOps();
};

