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
	for (int i = 0; i < n; i++)
		d0[i] = 0;

	while ((goal - endeffector).mag() > EPSILON && counter < MAX_ITERATIONS)
	{
		for (int i = 0; i < n; i++)
			JTs[i] = 0;

		GenMatrix<float, MAX_BONES * 3, 3>& JT = artbody.getJacobian(goal);

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
			{
				JTJ[i][j] = 0;
				for (int k = 0; k < 3; k++)
					JTJ[i][j] += JT(i, k) * JT(j, k);
			}

		
		#ifdef DAMPED_LEAST_SQUARES
		for (int i = 0; i < n; i++)
			JTJ[i][i] += DAMPED_LEAST_SQUARES_DAMPING;
		#endif

		vector3d s = goal - endeffector;

		#ifdef ITERATIVE_DELTA_S
			s = s * ITERATIVE_DELTA_S_MAG;
		#endif

		for (int i = 0; i < n;i++)
			for (int j = 0; j < 3; j++)
				JTs[i] += JT(i, j) * s.coords[j];
		
		solver.solve(d0, JTs);
		artbody.solverRotate(d0);

		//for (int i = 0; i < n; i++)
		//	printf("%f ", d0[i]);


		endeffector = artbody.getEndEffector();
		++counter;
	}

}

void IKSolver::solveByCCD(vector3d& goal)
{
	int counter = 0;
	float eps = (goal - artbody.getEndEffector()).mag();
	
	while (eps > EPSILON && counter < MAX_ITERATIONS)
	{
		bone* pbone = artbody.getEndEffectorBone();
		while (pbone)
		{
			vector3d& endeffector = artbody.getEndEffector();
			vector3d& root = pbone->getPosition();
			
			vector3d rootToEnd = endeffector - root;
			vector3d rootToGoal = goal - root;
			
			vector3d axis = (rootToEnd.cross(rootToGoal)).unit();
			
			float endgoalprod = rootToEnd.unit().dot(rootToGoal.unit());
			if (fabs(endgoalprod) >= 1.0)
			{
				pbone = pbone->getParent();
				continue;
			}
			float angle = acos(endgoalprod);
			pbone->rotate(quatn(axis,angle));
			pbone = pbone->getParent();
		}

		eps = (goal - artbody.getEndEffector()).mag();
		counter++;
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
