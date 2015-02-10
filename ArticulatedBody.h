#pragma once
#include "BallJointBone.h"
#include "HingeJointBone.h"

class ArticulatedBody
{
	QuickArray<bone*, MAX_BONES> bones;
public:
	ArticulatedBody();
	~ArticulatedBody();
};

