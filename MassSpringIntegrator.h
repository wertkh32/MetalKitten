#pragma once
#include "MassSpringMesh.h"
#define DAMPING 0.1

class MassSpringIntegrator
{
	int n;
	int s;
	int dim;
	int dim_s;


	float* d;
	float* fext;
	float* x;
	float* qn, *qn_1;
	float** A;
	float** J;
	MassSpringMesh* mesh;

	void init_d();
	void init_x();

public:
	MassSpringIntegrator(MassSpringMesh* _mesh);
	
	void solve_d();
	void solve_x();

	void solve();

	~MassSpringIntegrator();
};

