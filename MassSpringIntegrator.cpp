#include "MassSpringIntegrator.h"


MassSpringIntegrator::MassSpringIntegrator(MassSpringMesh* _mesh) : mesh(_mesh)
{
	n = mesh->getNoMassPoints();
	s = mesh->getNoSprings();
	dim = n * 3;
	dim_s = s * 3;

	d = (float*)calloc(dim_s, sizeof(float));

	fext = (float*)calloc(dim, sizeof(float));
	qn = (float*)calloc(dim, sizeof(float));
	qn_1 = (float*)calloc(dim, sizeof(float));
	x = (float*)calloc(dim, sizeof(float));
	
	A = (float**)calloc(dim, sizeof(float*));
	for (int i = 0; i < dim; i++)
		A[i] = (float*)calloc(dim, sizeof(float));

	J = (float**)calloc(dim, sizeof(float*));
	for (int i = 0; i < dim; i++)
		J[i] = (float*)calloc(dim_s, sizeof(float));

	mesh->getSystemMatrix(A);
	mesh->getJ(J);

	init_x();
	init_d();
}

void
MassSpringIntegrator::init_x()
{
	for (int i = 0; i < n; i++)
	{
		vector3d& v = mesh->getMassPoint(i).vertex;
		x[i * 3] = v.x;
		x[i * 3 + 1] = v.y;
		x[i * 3 + 2] = v.z;
	}
}

void
MassSpringIntegrator::init_d()
{
	for (int i = 0; i < s; i++)
	{
		Spring& spring = mesh->getSpring(i);
		vector3d dd = (mesh->getMassPoint(spring.v0).vertex - mesh->getMassPoint(spring.v1).vertex).unit() * spring.restLength;

		d[i * 3] = dd.x;
		d[i * 3 + 1] = dd.y;
		d[i * 3 + 2] = dd.z;
	}
}

void
MassSpringIntegrator::solve_x()
{

}

void
MassSpringIntegrator::solve_d()
{

}


MassSpringIntegrator::~MassSpringIntegrator()
{
	free(d);
	free(fext);

	free(qn);
	free(qn_1);
	free(x);

	for (int i = 0; i < dim; i++)
		free(A[i]);
	free(A);

	
	for (int i = 0; i < dim; i++)
		free(J[i]);
	free(J);
}
