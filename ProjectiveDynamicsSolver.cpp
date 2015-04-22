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
	}


	qn = (float*)calloc(numdof, sizeof(float));
	q = (float*)calloc(numdof, sizeof(float));
	v = (float*)calloc(numdof, sizeof(float));
	sn = (float*)calloc(numdof, sizeof(float));
	fext = (float*)calloc(numdof, sizeof(float));
	b = (float*)calloc(numdof, sizeof(float));
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


		for (int i = 0; i < 3; i++)
		{
			float tetweight = t.weight * t.volume;

			LTL[i0 * 3 + i][i0 * 3 + i] += 1 * tetweight;
			LTL[i1 * 3 + i][i1 * 3 + i] += 1 * tetweight;
			LTL[i2 * 3 + i][i2 * 3 + i] += 1 * tetweight;
			LTL[i3 * 3 + i][i3 * 3 + i] += 3 * tetweight;

			LTL[i0 * 3 + i][i3 * 3 + i] += -1 * tetweight;
			LTL[i1 * 3 + i][i3 * 3 + i] += -1 * tetweight;
			LTL[i2 * 3 + i][i3 * 3 + i] += -1 * tetweight;

			LTL[i3 * 3 + i][i0 * 3 + i] += -1 * tetweight;
			LTL[i3 * 3 + i][i1 * 3 + i] += -1 * tetweight;
			LTL[i3 * 3 + i][i2 * 3 + i] += -1 * tetweight;

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
		M[i * 3 + 1][i * 3 + 2] = mass;
		M[i * 3 + 1][i * 3 + 2] = mass;
	}
}

void
ProjectiveDynamicsSolver::initSystemMatrix()
{
	for (int i = 0; i < numdof;i++)
		for (int j = 0; j < numdof; j++)
			A[i][j] = M[i][j] * (1.0 / DT) * (1.0 / DT) + LTL[i][j];
}



void
ProjectiveDynamicsSolver::init(float* initv, float* initq)
{
	initLaplacian();
	initMass();
	initSystemMatrix;

	MatrixOps::InverseMatrix(A, Ainv, numdof);

	for (int i = 0; i < numdof; i++)
	{
		v[i] = initv[i];
		q[i] = initq[i];
		qn[i] = initq[i];
	}

}

void
ProjectiveDynamicsSolver::timestep()
{
	for (int i = 0; i < numdof; i++)
	{
		qn[i] = q[i];
		sn[i] = qn[i] + DT * v[i] + DT * DT + fext[i] / M[i][i];
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

			for (int j; j < 3; j++)
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
				q[i] += A[i][j] * b[j];
		}

		//to be continued
	}
}


ProjectiveDynamicsSolver::~ProjectiveDynamicsSolver()
{
}
