#pragma once
#include "MatrixOps.h"
#include "MassSpringMesh.h"
#include "ClothCollider.h"
#define DAMPING 0.9995
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

	bool* constrained;

	QuickArray<int, 100> constraints;

	MassSpringMesh* mesh;

	void init_d();
	void init_x();
	void init_q();
	void removeConstrainedRows();

	void solve_d();
	void solve_x();

	void reset_constrained_x();

public:
	ClothCollider collider;

	MassSpringIntegrator(MassSpringMesh* _mesh);

	void addConstrainedDOF(int n){ constraints.push(n); }
	void initSolver();
	void addExtForce(int n, vector3d& f);
	void timeStep();


	~MassSpringIntegrator();
};

