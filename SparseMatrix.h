#pragma once
#include "includes.h"

class SparseMatrix
{
	float* entries;
	int* indices;
	int numentries;
	int n;

	void processMatrix(float** A);

public:
	SparseMatrix(float** A, int numnodes);
	void mulXCompressed3x3(float* in, float* out);
	~SparseMatrix();
};

