#pragma once
#include "BallJointBone.h"
#include "HingeJointBone.h"
#include "GenMatrix.h" 

class ArticulatedBody
{
protected:
	QuickArray<bone*, MAX_BONES> bones;
	GenMatrix<float, MAX_BONES * 3, 3> jacobian;
	int no_rows;
	int end_index;
public:
	ArticulatedBody(){}
	int getNoBones(){ return bones.size(); }
	int getNoJacobianRows(){ return no_rows; }
	vector3d& getEndEffector(){ return bones[end_index]->getEndPosition(); }
	bone*	  getEndEffectorBone(){ return bones[end_index]; }
	QuickArray<bone*, MAX_BONES>& getBones(){ return bones; }
	
	~ArticulatedBody();
	virtual void render() = 0;
	virtual GenMatrix<float, MAX_BONES * 3, 3>& getJacobian(vector3d& goal) = 0; //returns JT
	virtual void solverRotate(float* d0) = 0;
};

