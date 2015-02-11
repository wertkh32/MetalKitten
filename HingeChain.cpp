#include "HingeChain.h"


HingeChain::HingeChain(vector3d* arr,vector3d*axes, int n)
{
	int nn = MIN(n - 1, MAX_BONES);

	for (int i = 0; i < nn; i++)
		bones.push(new HingeJointBone(arr[i], arr[i + 1],axes[i]));

	for (int i = 1; i < nn; i++)
		bones[i-1]->addChild(bones[i]);

	end_index = getNoBones() - 1;
	no_rows = nn;
}

GenMatrix<float, MAX_BONES * 3, 3>& HingeChain::getJacobian(vector3d& goal)
{
	vector3d& chain_end = getEndEffector();
	for (int i = 0; i < getNoBones(); i++)
	{
		vector3d bonejac = ((HingeJointBone*)(bones[i]))->getJacobianRow(chain_end);
		for (int j = 0; j < 3; j++)
			jacobian(i, j) = bonejac.coords[j];
	}

	return jacobian;
}

void HingeChain::solverRotate(float* d0)
{
	for (int i = 0; i < getNoBones(); i++)
	{
		((HingeJointBone*)bones[i])->rotate(d0[i]);
	}
}

void HingeChain::render()
{
	for (int i = 0; i < getNoBones(); i++)
		bones[i]->render();
}

HingeChain::~HingeChain(void)
{
}
