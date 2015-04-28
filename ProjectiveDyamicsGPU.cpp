#include "ProjectiveDyamicsGPU.h"

ProjectiveDynamicsGPU::ProjectiveDynamicsGPU(TetMesh* _tetmesh) : tetmesh(_tetmesh)
{
	int n = tetmesh->getNumNodes();
	if(n%16 != 0)
	{
		int filler = 16 - n % 16;

		for(int i=0;i<filler;i++)
			tetmesh->addNode(Node(vector3d(),1));
	}

	numnodes = tetmesh->getNumNodes();
	numtets = tetmesh->getNumTets();
	numdof = numnodes * 3;

	//LTL = (float**)calloc(numnodes, sizeof(float*));
	A = (float**)calloc(numnodes, sizeof(float*));
	//Ainv = (float**)calloc(numnodes, sizeof(float*));


	for (int i = 0; i < numnodes; i++)
	{
		//LTL[i] = (float*)calloc(numnodes, sizeof(float));
		A[i] = (float*)calloc(numnodes, sizeof(float));
		//Ainv[i] = (float*)calloc(numnodes, sizeof(float));

		if( A[i] == 0 ) printf(" NO SPACE ");
	}

	qn = (float*)calloc(numdof, sizeof(float));
	q0 = (float*)calloc(numdof, sizeof(float));
	q = (float*)calloc(numdof, sizeof(float));
	v = (float*)calloc(numdof, sizeof(float));
	sn = (float*)calloc(numdof, sizeof(float));
	fext = (float*)calloc(numdof, sizeof(float));
	b = (float*)calloc(numdof, sizeof(float));
	constrained = (char*)calloc(numnodes, sizeof(char));
	mass = (float*)calloc(numnodes, sizeof(float));


	numblockspertet = (numtets / TET_BLOCK_SIZE) + 1;
	numblockpernode = (numnodes / NODE_BLOCK_SIZE) + 1;
	
	tetdata = (TetData*)calloc(numblockspertet,sizeof(TetData));
	nodedata = (NodeData*)calloc(numblockpernode, sizeof(NodeData));
	max_entry = -1;

	fext_dirty = false;

}

void
ProjectiveDynamicsGPU::initLaplacian()
{
	for (int i = 0; i < numtets; i++)
	{
		Tet& t=tetmesh->getTet(i);
		
		int i0 = t.node[0];
		int i1 = t.node[1];
		int i2 = t.node[2];
		int i3 = t.node[3];

		float tetweight = t.weight * t.volume;

		A[i0][i0] += 1 * tetweight;
		A[i1][i1] += 1 * tetweight;
		A[i2][i2] += 1 * tetweight;
		A[i3][i3] += 3 * tetweight;

		A[i0][i3] += -1 * tetweight;
		A[i1][i3] += -1 * tetweight;
		A[i2][i3] += -1 * tetweight;

		A[i3][i0] += -1 * tetweight;
		A[i3][i1] += -1 * tetweight;
		A[i3][i2] += -1 * tetweight;

	}

}

void
ProjectiveDynamicsGPU::initMass()
{
	for (int i = 0; i < numnodes; i++)
	{
		float _mass = tetmesh->getNode(i).mass;
		mass[i] = _mass;
	
	}
}

void
ProjectiveDynamicsGPU::initSystemMatrix()
{
	for (int i = 0; i < numnodes;i++)
		A[i][i] += mass[i] / (DT * DT);
		
}



void
ProjectiveDynamicsGPU::init()
{
	initLaplacian();
	initMass();
	initSystemMatrix();

	int count = 0;
	max_entry = -1;

	//generate entries
	for (int i = 0; i < numnodes; i++)
	{
		int blockid = i / NODE_BLOCK_SIZE;
		int ltid = i % NODE_BLOCK_SIZE;

		int rowcount = 0;
		
		for (int j = 0; j < numnodes; j++)
		{
			if (fabs(A[i][j]) >= 1e-9)
			{
				nodedata[blockid].nodeEntries[rowcount][ltid] = A[i][j];
				nodedata[blockid].nodeEntryIndex[rowcount][ltid] = j;
				rowcount++;

			}
			else
				count++;
		}
		max_entry = MAX(max_entry,rowcount);

		int tetcount = 0;
		for(int j=0;j<numtets;j++)
		{
			for(int k=0;k<4;k++)
				if(tetmesh->getTet(j).node[k] == i)
				{
					nodedata[blockid].indices[tetcount][TET_INDEX][ltid] = j;
					nodedata[blockid].indices[tetcount][NODE_INDEX][ltid] = k;
					tetcount++;
				}
		}

		nodedata[blockid].ntets[ltid] = tetcount;

	}
	printf("\n%d %d %d", count,numnodes * numnodes, max_entry);


	for (int i = 0; i < numtets; i++)
	{
		int blockid = i / TET_BLOCK_SIZE;
		int ltid = i % TET_BLOCK_SIZE;

		TetData& td = tetdata[blockid];
		Tet& t = tetmesh->getTet(i);

		for(int j=0;j<3;j++)
			for(int k=0;k<3;k++)
			{
				td.Dm[j][k][ltid] = t.Dm(j,k);
				td.Bm[j][k][ltid] = t.Bm(j,k);
			}

			td.index[0][ltid] = t.node[0];
			td.index[1][ltid] = t.node[1];
			td.index[2][ltid] = t.node[2];
			td.index[3][ltid] = t.node[3];

			td.weight[ltid] = t.weight * t.volume;
	}

	for (int i = 0; i < numdof; i++)
		v[i] = 0;

	for (int i = 0; i < numnodes; i++)
	{
		qn[i] = q[i] = tetmesh->getNode(i).position.x;
		qn[i + numnodes] = q[i + numnodes] = tetmesh->getNode(i).position.y;
		qn[i + numnodes * 2] = q[i + numnodes * 2] = tetmesh->getNode(i).position.z;

		q0[i] =  tetmesh->getRestPosition(i).x;
		q0[i + numnodes] =  tetmesh->getRestPosition(i).y;
		q0[i + numnodes * 2] =  tetmesh->getRestPosition(i).z;
	}


	gpuInitVars(numtets, numnodes);
	gpuUploadVars(tetdata, nodedata, q, q0, v, fext, mass, constrained, numnodes, numtets);
}

void
ProjectiveDynamicsGPU::setPosition(int nodeindex, vector3d pos)
{
	q[nodeindex] = pos.x;
	q[nodeindex  + numnodes] = pos.y;
	q[nodeindex  + numnodes * 2] = pos.z;

	qn[nodeindex] = pos.x;
	qn[nodeindex + numnodes] = pos.y;
	qn[nodeindex  + numnodes * 2] = pos.z;

	tetmesh->getNode(nodeindex).position = pos;
}

void
ProjectiveDynamicsGPU::setVelocity(int nodeindex, vector3d vel)
{
	v[nodeindex] = vel.x;
	v[nodeindex + numnodes] = vel.y;
	v[nodeindex + numnodes * 2] = vel.z;

}
void
ProjectiveDynamicsGPU::setExtForce(int nodeindex, vector3d force)
{
	fext_dirty = true;

	fext[nodeindex] = force.x;
	fext[nodeindex + numnodes] = force.y;
	fext[nodeindex + numnodes * 2] = force.z;
}

void
ProjectiveDynamicsGPU::addExtForce(int nodeindex, vector3d force)
{
	fext_dirty = true;

	fext[nodeindex] += force.x;
	fext[nodeindex + numnodes] += force.y;
	fext[nodeindex + numnodes * 2] += force.z;
}


void
ProjectiveDynamicsGPU::timestep()
{
	if(fext_dirty)
		gpuUploadExtForces(fext, numnodes);

	GPUTimestep(numtets, numnodes, max_entry);

	gpuDownloadVars(q, numnodes);

	for (int i = 0; i < numnodes; i++)
	{
		tetmesh->getNode(i).position = vector3d(q[i], q[i + numnodes], q[i  + numnodes * 2]);

		fext[i] = 0;
		fext[i + numnodes] = 0;
		fext[i + numnodes * 2] = 0;
	}

	fext_dirty = false;
}


ProjectiveDynamicsGPU::~ProjectiveDynamicsGPU()
{
	gpuDestroyVars();
}
