#pragma once
#include  "TetMesh.h"
#include "MatrixOps.h"

#define MAX_ITERATIONS 10

class ProjectiveDynamicsSolver
{
	TetMesh* tetmesh;
	int numnodes;
	int numtets;
	int numdof;

	float** M;
	float** LTL;
	float** A;
	float** Ainv;
	float *qn, *q, *sn, *v, *fext, *b;


	void initLaplacian();
	void initMass();
	void initSystemMatrix();


public:
	ProjectiveDynamicsSolver(TetMesh* _tetmesh);

	void init(float* initv, float* initq);
	void timestep();


	~ProjectiveDynamicsSolver();
};

