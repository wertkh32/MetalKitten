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

	for (int i = 0; i < nv * 3;i++)
		for (int j = 0; j < nv * 3; j++)
			A[i][j] *= DT * DT;

	for (int i = 0; i < nv; i++)
	{
		float mass = getMassPoint(i).mass;
		A[i * 3][i * 3]			+=  mass;
		A[i * 3 + 1][i * 3 + 1] += mass;
		A[i * 3 + 2][i * 3 + 2] += mass;
	}
	

}

void
MassSpringMesh::render()
{
	int s = getNoSprings();
	int nv = getNoMassPoints();

	for (int i = 0; i < nv; i++)
	{
		vector3d& v = getMassPoint(i).vertex;

		glPushMatrix();
		glTranslatef(v.x, v.y, v.z);
		glutSolidSphere(0.5, 10, 10);
		glPopMatrix();
	}

	glPushMatrix();
	glBegin(GL_LINES);
	glLineWidth(3.0f);
	for (int i = 0; i < s; i++)
	{
		Spring& s = getSpring(i);
		vector3d& v0 = getMassPoint(s.v0).vertex;
		vector3d& v1 = getMassPoint(s.v1).vertex;

		glVertex2fv(v0.coords);
		glVertex2fv(v1.coords);
	}
	glEnd();
	glPopMatrix();

}


MassSpringMesh::~MassSpringMesh()
{
}
