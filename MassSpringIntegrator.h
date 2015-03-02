#pragma once
#include "MatrixOps.h"
#include "MassSpringMesh.h"
#define DAMPING 0.95
#define MAX_ITERATION 5

class MassSpringIntegrator
{
	int n;
	int s;
	int dim;
	int dim_s;


	float* d;
	float* fext;
	float* x;
	float* b;
	float* temp;

	float* qn, *qn_1;

	float** A;
	float** J;
	float** Ainv;

	MassSpringMesh* mesh;

	void init_d();
	void init_x();
	void init_q();

	void solve_d();
	void solve_x();

public:
	MassSpringIntegrator(MassSpringMesh* _mesh);

	void addExtForce(int n, vector3d& f);
	void timeStep();


	~MassSpringIntegrator();
};

