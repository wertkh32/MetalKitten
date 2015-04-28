#pragma once
#include "PDGPUKernel.cuh"
#include  "TetMesh.h"
#include "MatrixOps.h"
#include "ConjugateGradientSolver.h"

#define DAMPING 0.98

class ProjectiveDynamicsGPU
{
	TetMesh* tetmesh;
	int numnodes;
	int numtets;
	int numdof;
	int numblockspertet;
	int numblockpernode;
	int max_entry;

	//float** LTL;
	float** A;
	//float** Ainv;
	float *qn, *q, *sn, *v, *fext, *b, *mass, *q0;
	char* constrained;
	bool fext_dirty;


	NodeData* nodedata;
	TetData* tetdata;
	
	void initLaplacian();
	void initMass();
	void initSystemMatrix();


public:
	ProjectiveDynamicsGPU(TetMesh* _tetmesh);

	void init();

	void timestep();

	void setPosition(int nodeindex, vector3d pos);
	void setVelocity(int nodeindex, vector3d vel);
	void setExtForce(int nodeindex, vector3d force);
	void addExtForce(int nodeindex, vector3d force);
	void setContrainedNode(int node, bool isConstrained){ constrained[node] = isConstrained ? (char)1 : (char)0; }

	~ProjectiveDynamicsGPU();
};

