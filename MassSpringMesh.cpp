#include "MassSpringMesh.h"


MassSpringMesh::MassSpringMesh()
{
}


void
MassSpringMesh::getStiffnessWeightedLaplacian(float** A)
{
	int s = getNoSprings();
	int nv = getNoMassPoints();

	for (int i = 0; i < nv * 3;i++)
		for (int j = 0; j < nv * 3; j++)
			A[i][j] = 0;

	for (int i = 0; i < s; i++)
	{
		Spring& e = getSpring(i);
		
		A[e.v0 * 3][e.v0 * 3]		  += e.stiffness;
		A[e.v0 * 3 + 1][e.v0 * 3 + 1] += e.stiffness;
		A[e.v0 * 3 + 2][e.v0 * 3 + 2] += e.stiffness;

		A[e.v1 * 3][e.v1 * 3]		  += e.stiffness;
		A[e.v1 * 3 + 1][e.v1 * 3 + 1] += e.stiffness;
		A[e.v1 * 3 + 2][e.v1 * 3 + 2] += e.stiffness;

		A[e.v0 * 3][e.v1 * 3]		  = -e.stiffness;
		A[e.v0 * 3 + 1][e.v1 * 3 + 1] = -e.stiffness;
		A[e.v0 * 3 + 2][e.v1 * 3 + 2] = -e.stiffness;

		A[e.v1 * 3][e.v0 * 3]		  = -e.stiffness;
		A[e.v1 * 3 + 1][e.v0 * 3 + 1] = -e.stiffness;
		A[e.v1 * 3 + 2][e.v0 * 3 + 2] = -e.stiffness;
	}
}

void
MassSpringMesh::getJ(float** J)
{
	int s = getNoSprings();
	int nv = getNoMassPoints();

	for (int i = 0; i < nv * 3; i++)
		for (int j = 0; j < s * 3; j++)
			J[i][j] = 0;

	for (int i = 0; i < s; i++)
	{
		Spring& e = getSpring(i);

		J[e.v0 * 3][i * 3]		   = e.stiffness;
		J[e.v0 * 3 + 1][i * 3 + 1] = e.stiffness;
		J[e.v0 * 3 + 2][i * 3 + 2] = e.stiffness;

		J[e.v1 * 3][i * 3]		   = -e.stiffness;
		J[e.v1 * 3 + 1][i * 3 + 1] = -e.stiffness;
		J[e.v1 * 3 + 2][i * 3 + 2] = -e.stiffness;
	}
}

void
MassSpringMesh::getSystemMatrix(float** A)
{
	int nv = getNoMassPoints();

	getStiffnessWeightedLaplacian(A);

	for (int i = 0; i < nv; i++)
	{
		float diag = getMassPoint(i).mass * DT * DT;
		A[i * 3][i * 3]			+= diag;
		A[i * 3 + 1][i * 3 + 1] += diag;
		A[i * 3 + 2][i * 3 + 2] += diag;
	}
	

}


MassSpringMesh::~MassSpringMesh()
{
}
