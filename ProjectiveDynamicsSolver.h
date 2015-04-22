#pragma once
#include  "TetMesh.h"
#include "MatrixOps.h"

#define MAX_ITERATIONS 20
#define DAMPING 0.99

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
	bool* constrained;

	void initLaplacian();
	void initMass();
	void initSystemMatrix();


public:
	ProjectiveDynamicsSolver(TetMesh* _tetmesh);

	void init();
	void timestep();

	void setPosition(int nodeindex, vector3d pos);
	void setVelocity(int nodeindex, vector3d vel);
	void setExtForce(int nodeindex, vector3d force);
	void setContrainedNode(int node, bool isConstrained){ constrained[node] = isConstrained; }

	~ProjectiveDynamicsSolver();
};

