#include "Chain.h"


Chain::Chain(vector3d* arr, int n)
{
	int nn = MIN(n - 1, MAX_BONES);

	for (int i = 0; i < nn; i++)
		bones.push(new BallJointBone(arr[i], arr[i + 1]));

	for (int i = 1; i < nn; i++)
		bones[i-1]->addChild(bones[i]);

	end_index = getNoBones() - 1;
	#ifdef USE_QUATN_JACOBIAN
		no_rows = nn;
	#else
		no_rows = nn * 3;
	#endif
}

GenMatrix<float, MAX_BONES * 3, 3>& Chain::getJacobian(vector3d& goal)
{
	vector3d& chain_end = getEndEffector();

	#ifdef USE_QUATN_JACOBIAN
		quatn_axes.reset();
	#endif

	for (int i = 0; i < getNoBones(); i++)
	{
		#ifdef USE_QUATN_JACOBIAN
			vector3d _axis;
			vector3d bonejac = ((BallJointBone*)(bones[i]))->getJacobianQuatn(chain_end,goal,_axis);
			quatn_axes.push(_axis);
			for (int j = 0; j < 3; j++)
				jacobian(i, j) = bonejac.coords[j];
		#else
			Matrix3d bonejac = ((BallJointBone*)(bones[i]))->getJacobian3DOF(chain_end);
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					jacobian(i * 3 + j, k) = bonejac(j, k);
		#endif
		
	}

	return jacobian;
}

void Chain::solverRotate(float* d0)
{
	for (int i = 0; i < getNoBones(); i++)
	{
		#ifdef USE_QUATN_JACOBIAN
			((BallJointBone*)bones[i])->rotate(quatn(quatn_axes[i],d0[i]));
		#else
			vector3d angles = vector3d(d0[i * 3], d0[i * 3 + 1], d0[i * 3 + 2]);
			((BallJointBone*)bones[i])->rotate(angles);
		#endif
	
	}
}

void Chain::render()
{
	for (int i = 0; i < getNoBones(); i++)
		bones[i]->render();
}

Chain::~Chain()
{
	
}
