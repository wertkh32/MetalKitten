#pragma once
#include "includes.h"
#include "Matrix3d.h"

#define TOLERANCE 0.001
class PolarDecompose
{
public:
	static float oneNorm(Matrix3d& F);
	static float infNorm(Matrix3d& F);
	static void compute(Matrix3d& F, Matrix3d& R, Matrix3d& S);
	static void computeFull(Matrix3d& F, Matrix3d& R, Matrix3d& S);
};

