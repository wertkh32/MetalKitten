#include "MassSpringIntegrator.h"


MassSpringIntegrator::MassSpringIntegrator(MassSpringMesh* _mesh) : mesh(_mesh), collider(_mesh)
{

}

void
MassSpringIntegrator::initSolver()
{
	n = mesh->getNoMassPoints();
	s = mesh->getNoSprings();
	dim = n * 3;
	dim_s = s * 3;

	collider.initCollider();

	d = (float*)calloc(dim_s, sizeof(float));


	fext = (float*)calloc(dim, sizeof(float));
	qn = (float*)calloc(dim, sizeof(float));
	qn_1 = (float*)calloc(dim, sizeof(float));
	x = (float*)calloc(dim, sizeof(float));
	b = (float*)calloc(dim, sizeof(float));
	temp = (float*)calloc(dim, sizeof(float));

	constrained = (bool*)calloc(n, sizeof(bool));


	A = (float**)calloc(dim, sizeof(float*));
	for (int i = 0; i < dim; i++)
		A[i] = (float*)calloc(dim, sizeof(float));

	J = (float**)calloc(dim, sizeof(float*));
	for (int i = 0; i < dim; i++)
		J[i] = (float*)calloc(dim_s, sizeof(float));

	Ainv = (float**)calloc(dim, sizeof(float*));
	for (int i = 0; i < dim; i++)
		Ainv[i] = (float*)calloc(dim, sizeof(float));


	for (int i = 0; i < constraints.size(); i++)
		constrained[constraints[i]] = true;


	mesh->getSystemMatrix(A);
	mesh->getJ(J);

	//for (int i = 0; i < dim; i++)
	//{
	//	printf("\n");
	//	for (int j = 0; j < dim; j++)
	//		printf("%f ", A[i][j]);
	//}

	//printf("\n");

	MatrixOps::InverseMatrix(A, Ainv, dim);


	//for (int i = 0; i < dim; i++)
	//{
	//	printf("\n");
	//	for (int j = 0; j < dim; j++)
	//		printf("%f ", Ainv[i][j]);
	//}

	init_x();
	init_d();
	init_q();
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
MassSpringIntegrator::init_q()
{
	for (int i = 0; i < dim; i++)
		qn_1[i] = qn[i] = x[i];
}

void
MassSpringIntegrator::reset_constrained_x()
{
	for (int i = 0; i < n; i++)
	{
		if (constrained[i])
		{
			vector3d& v = mesh->getMassPoint(i).vertex;
			x[i * 3] = v.x;
			x[i * 3 + 1] = v.y;
			x[i * 3 + 2] = v.z;
		}
	}
}

void
MassSpringIntegrator::addExtForce(int n, vector3d& f)
{
	fext[n * 3] += f.x;
	fext[n * 3 + 1] += f.y;
	fext[n * 3 + 2] += f.z;
}

void
MassSpringIntegrator::solve_x()
{
	for (int i = 0; i < dim; i++)
	{
		if (constrained[i / 3]) continue;
		x[i] = 0;
		for (int j = 0; j < dim; j++)
			x[i] += Ainv[i][j] * b[j];
	}
}

void
MassSpringIntegrator::solve_d()
{
	for (int i = 0; i < s; i++)
	{
		Spring& spring = mesh->getSpring(i);
		int v0 = spring.v0;
		int v1 = spring.v1;
		vector3d dd = vector3d(x[v0 * 3] - x[v1 * 3], x[v0 * 3 + 1] - x[v1 * 3 + 1], x[v0 * 3 + 2] - x[v1 * 3 + 2]).unit() * spring.restLength;

		d[i * 3] = dd.x;
		d[i * 3 + 1] = dd.y;
		d[i * 3 + 2] = dd.z;
	}
}

void
MassSpringIntegrator::timeStep()
{

	for (int i = 0; i < dim; i++)
	{
		qn_1[i] = qn[i];
		qn[i] = x[i];
	}

	for (int i = 0; i < dim; i++)
		temp[i] = -fext[i] * DT * DT;

	for (int i = 0; i < n; i++)
	{
		float mass = mesh->getMassPoint(i).mass;
		temp[i * 3] += mass * ((1 + DAMPING) * qn[i * 3] - DAMPING * qn_1[i * 3]);
		temp[i * 3 + 1] += mass * ((1 + DAMPING) * qn[i * 3 + 1] - DAMPING * qn_1[i * 3 + 1]);
		temp[i * 3 + 2] += mass * ((1 + DAMPING) * qn[i * 3 + 2] - DAMPING * qn_1[i * 3 + 2]);
	}

	int counter = 0;

	//add some energy cutoff
	while (counter < MAX_ITERATION)
	{
		for (int i = 0; i < dim; i++)
		{
			b[i] = 0;
			for (int j = 0; j < dim_s; j++)
				b[i] += J[i][j] * d[j];
			b[i] *= DT *DT;
			b[i] += temp[i];
		}

		solve_x();

		#ifdef COLLISION
		collider.applyCollisionConstraint(x, qn, qn_1);
		#endif
		reset_constrained_x();

		solve_d();

		counter++;
	}

	for (int i = 0; i < n; i++)
	{
		vector3d& v = mesh->getMassPoint(i).vertex;
		
		v.x = x[i * 3];
		v.y = x[i * 3 + 1];
		v.z = x[i * 3 + 2];

		//printf("%f %f %f\n", v.x, v.y, v.z);
	}

	for (int i = 0; i < dim; i++)
		fext[i] = 0;
}


MassSpringIntegrator::~MassSpringIntegrator()
{
	free(d);
	free(fext);

	free(qn);
	free(qn_1);
	free(x);
	free(b);
	free(temp);
	free(constrained);

	for (int i = 0; i < dim; i++)
		free(Ainv[i]);
	free(Ainv);


	for (int i = 0; i < dim; i++)
		free(A[i]);
	free(A);

	
	for (int i = 0; i < dim; i++)
		free(J[i]);
	free(J);
}



//void
//MassSpringIntegrator::backwardSub(float* b)
//{
//for (int i = dim - 1; i >= 0; i--)
//{
//	float xx = b[i];
//	for (int j = i + 1; j < dim; j--)
//			xx -= LT[i][j] * z[j];
//	z[i] = xx / LT[i][i];
//}
//}

//void
//MassSpringIntegrator::forwardSub(float* b)
//{
//for (int i = 0; i < dim; i++)
//{
//	float xx = b[i];
//	for (int j = 0; j < i; j--)
//		xx -= LT[j][i] * x[j];
//	x[i] = xx / LT[i][i];
//}
//}