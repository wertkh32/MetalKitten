#include "ProjectiveDynamicsSolver.h"

ProjectiveDynamicsSolver::ProjectiveDynamicsSolver(TetMesh* _tetmesh) : tetmesh(_tetmesh)
{
	numnodes = tetmesh->getNumNodes();
	numtets = tetmesh->getNumTets();
	numdof = numnodes * 3;


	M = (float**)calloc(numdof,sizeof(float*));
	LTL = (float**)calloc(numdof,sizeof(float*));
	A = (float**)calloc(numdof, sizeof(float*));
	Ainv = (float**)calloc(numdof, sizeof(float*));


	for (int i = 0; i < numdof; i++)
	{
		M[i] = (float*)calloc(numdof, sizeof(float));
		LTL[i] = (float*)calloc(numdof, sizeof(float));
		A[i] = (float*)calloc(numdof, sizeof(float));
		Ainv[i] = (float*)calloc(numdof, sizeof(float));
	}


	qn = (float*)calloc(numdof, sizeof(float));
	q = (float*)calloc(numdof, sizeof(float));
	v = (float*)calloc(numdof, sizeof(float));
	sn = (float*)calloc(numdof, sizeof(float));
	fext = (float*)calloc(numdof, sizeof(float));
	b = (float*)calloc(numdof, sizeof(float));
	constrained = (bool*)calloc(numnodes, sizeof(bool));
}

void
ProjectiveDynamicsSolver::initLaplacian()
{
	for (int i = 0; i < numtets; i++)
	{
		Tet& t=tetmesh->getTet(i);
		
		int i0 = t.node[0];
		int i1 = t.node[1];
		int i2 = t.node[2];
		int i3 = t.node[3];


		for (int j = 0; j < 3; j++)
		{
			float tetweight = t.weight * t.volume;

			LTL[i0 * 3 + j][i0 * 3 + j] += 1 * tetweight;
			LTL[i1 * 3 + j][i1 * 3 + j] += 1 * tetweight;
			LTL[i2 * 3 + j][i2 * 3 + j] += 1 * tetweight;
			LTL[i3 * 3 + j][i3 * 3 + j] += 3 * tetweight;

			LTL[i0 * 3 + j][i3 * 3 + j] += -1 * tetweight;
			LTL[i1 * 3 + j][i3 * 3 + j] += -1 * tetweight;
			LTL[i2 * 3 + j][i3 * 3 + j] += -1 * tetweight;

			LTL[i3 * 3 + j][i0 * 3 + j] += -1 * tetweight;
			LTL[i3 * 3 + j][i1 * 3 + j] += -1 * tetweight;
			LTL[i3 * 3 + j][i2 * 3 + j] += -1 * tetweight;

		}

	}

}

void
ProjectiveDynamicsSolver::initMass()
{
	for (int i = 0; i < numnodes; i++)
	{
		float mass = tetmesh->getNode(i).mass;
		M[i * 3][i * 3] = mass;
		M[i * 3 + 1][i * 3 + 1] = mass;
		M[i * 3 + 2][i * 3 + 2] = mass;
	}
}

void
ProjectiveDynamicsSolver::initSystemMatrix()
{
	for (int i = 0; i < numdof;i++)
		for (int j = 0; j < numdof; j++)
		{
			A[i][j] = M[i][j] /(DT * DT) + LTL[i][j];
		}
}



void
ProjectiveDynamicsSolver::init()
{
	initLaplacian();
	initMass();
	initSystemMatrix();

	MatrixOps::InverseMatrix(A, Ainv, numdof);

	//for (int i = 0; i < numdof; i++)
	//{
	//	for (int j = 0; j < numdof; j++)
	//	{
			//printf("%f ", Ainv[i][j]);
	//	}
		//printf("\n");
	//}

	for (int i = 0; i < numdof; i++)
		v[i] = 0;

	for (int i = 0; i < numnodes; i++)
	{
		qn[i * 3] = q[i * 3] = tetmesh->getNode(i).position.x;
		qn[i * 3 + 1] = q[i * 3 + 1] = tetmesh->getNode(i).position.y;
		qn[i * 3 + 2] = q[i * 3 + 2] = tetmesh->getNode(i).position.z;
	}

}

void
ProjectiveDynamicsSolver::setPosition(int nodeindex, vector3d pos)
{
	q[nodeindex * 3] = pos.x;
	q[nodeindex * 3 + 1] = pos.y;
	q[nodeindex * 3 + 2] = pos.z;

	qn[nodeindex * 3] = pos.x;
	qn[nodeindex * 3 + 1] = pos.y;
	qn[nodeindex * 3 + 2] = pos.z;

	tetmesh->getNode(nodeindex).position = pos;
}

void
ProjectiveDynamicsSolver::setVelocity(int nodeindex, vector3d vel)
{
	v[nodeindex * 3] = vel.x;
	v[nodeindex * 3 + 1] = vel.y;
	v[nodeindex * 3 + 2] = vel.z;

}
void
ProjectiveDynamicsSolver::setExtForce(int nodeindex, vector3d force)
{
	fext[nodeindex * 3] = force.x;
	fext[nodeindex * 3 + 1] = force.y;
	fext[nodeindex * 3 + 2] = force.z;
}


void
ProjectiveDynamicsSolver::timestep()
{
	for (int i = 0; i < numdof; i++)
	{
		qn[i] = q[i];
		sn[i] = qn[i] + DT * v[i] * DAMPING + DT * DT * fext[i] / M[i][i];
		//clear external forces
		fext[i] = 0;
		//printf("%f ", sn[i]);
	}

	for (int n = 0; n < MAX_ITERATIONS; n++)
	{
		for (int i = 0; i < numdof; i++)
			b[i] = M[i][i] / (DT * DT) * sn[i];

		for (int i = 0; i < numtets; i++)
		{
			Tet& t = tetmesh->getTet(i);

			int i0 = t.node[0];
			int i1 = t.node[1];
			int i2 = t.node[2];
			int i3 = t.node[3];

			vector3d v0(q[i0 * 3], q[i0 * 3 + 1], q[i0 * 3 + 2]);
			vector3d v1(q[i1 * 3], q[i1 * 3 + 1], q[i1 * 3 + 2]);
			vector3d v2(q[i2 * 3], q[i2 * 3 + 1], q[i2 * 3 + 2]);
			vector3d v3(q[i3 * 3], q[i3 * 3 + 1], q[i3 * 3 + 2]);


			Matrix3d R = tetmesh->getRotation(i, tetmesh->generateD(v0,v1,v2,v3));

			Matrix3d edges = R * t.Dm * t.weight * t.volume;

			for (int j=0; j < 3; j++)
			{
				b[i0 * 3 + j] += edges(j, 0);
				b[i1 * 3 + j] += edges(j, 1);
				b[i2 * 3 + j] += edges(j, 2);
				b[i3 * 3 + j] += -edges(j, 0) - edges(j, 1) - edges(j, 2);
			}

		}

		for (int i = 0; i < numdof; i++)
		{
			
			q[i] = 0;
			for (int j = 0; j < numdof; j++)
				q[i] += Ainv[i][j] * b[j];

			//printf("%f ", q[i]);
		}

		for (int i=0;i<numnodes;i++)
		if (constrained[i])
		{
			setPosition(i, tetmesh->getRestPosition(i));
		}

		//to be continued
	}

	for (int i = 0; i < numdof; i++)
	{
		v[i] = (q[i] - qn[i]) / DT;
	}


	for (int i = 0; i < numnodes; i++)
	{
		tetmesh->getNode(i).position = vector3d(q[i * 3], q[i * 3 + 1], q[i * 3 + 2]);
	}
}


ProjectiveDynamicsSolver::~ProjectiveDynamicsSolver()
{
}
