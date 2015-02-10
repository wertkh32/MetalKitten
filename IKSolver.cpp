#include "IKSolver.h"


IKSolver::IKSolver(ArticulatedBody& _artbody) :artbody(_artbody)
{
	n = artbody.getNoJacobianRows();
	d0 = (float*)malloc(n * sizeof(float));
	JTs = (float*)malloc(n * sizeof(float));
	JTJ = (float**)malloc(n * sizeof(float*));
	for (int i = 0; i < n; i++)
		JTJ[i] = (float*)malloc(n * sizeof(float));

	solver.initSolver(n, JTJ);
}

void IKSolver::solveByJacobianInverse(vector3d& goal)
{

	vector3d endeffector = artbody.getEndEffector();
	int counter = 0;
	//printf("HELLO");
	while ((goal - endeffector).mag() > EPSILON && counter < MAX_ITERATIONS)
	{
		for (int i = 0; i < n; i++)
		{
			d0[i] = 0;
			JTs[i] = 0;
		}

		GenMatrix<float, MAX_BONES * 3, 3>& J = artbody.getJacobian();

		for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			JTJ[i][j] = 0;
			for (int k = 0; k < 3; k++)
				JTJ[i][j] += J(i, k) * J(j, k);
		}

		
		#ifdef DAMPED_LEAST_SQUARES
		for (int i = 0; i < n; i++)
			JTJ[i][i] += 1;
		#endif

		vector3d s = goal - endeffector;
		for (int i = 0; i < n;i++)
			for (int j = 0; j < 3; j++)
				JTs[i] += J(i, j) * s.coords[j];
		
		solver.solve(d0, JTs);
		artbody.solverRotate(d0);

		//for (int i = 0; i < n; i++)
		//	printf("%f ", d0[i]);


		endeffector = artbody.getEndEffector();
		++counter;
	}

}


IKSolver::~IKSolver()
{
	free(d0);
	free(JTs);
	for (int i = 0; i < n; i++)
		free(JTJ[i]);
	free(JTJ);
}
