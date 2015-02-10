#include "Chain.h"


Chain::Chain(vector3d* arr, int n)
{
	int nn = MIN(n - 1, MAX_BONES);

	for (int i = 0; i < nn; i++)
		bones.push(new BallJointBone(arr[i], arr[i + 1]));

	for (int i = 1; i < nn; i++)
		bones[i-1]->addChild(bones[i]);

	end_index = getNoBones() - 1;
	no_rows = nn * 3;
}

GenMatrix<float, MAX_BONES * 3, 3>& Chain::getJacobian()
{
	vector3d& chain_end = getEndEffector();
	for (int i = 0; i < getNoBones(); i++)
	{
		Matrix3d bonejac = ((BallJointBone*)(bones[i]))->getJacobian3DOF(chain_end);
		for (int j = 0; j < 3; j++)
		for (int k = 0; k < 3; k++)
			jacobian(i * 3 + j, k) = bonejac(j, k);
	}

	return jacobian;
}

void Chain::solverRotate(float* d0)
{
	for (int i = 0; i < getNoBones(); i++)
	{
		vector3d angles = vector3d(TO_DEG(d0[i * 3]), TO_DEG(d0[i * 3 + 1]), TO_DEG(d0[i * 3 + 2]));
		((BallJointBone*)bones[i])->rotate(angles);
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
