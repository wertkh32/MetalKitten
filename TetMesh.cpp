#include "TetMesh.h"


TetMesh::TetMesh()
{
}

void
TetMesh::getTransforms(int tetindex, Matrix3d& Ds, Matrix3d& F, Matrix3d& R, Matrix3d& S)
{
	Tet& t = tets[tetindex];

	int i0 = t.node[0];
	int i1 = t.node[1];
	int i2 = t.node[2];
	int i3 = t.node[3];

	Matrix3d DmInv = t.Dm.inverse();

	F = Ds * DmInv;

	PolarDecompose::computeFull(F, R, S);
}

Matrix3d
TetMesh::getRotation(int tetindex, Matrix3d& Ds)
{
	Tet& t = tets[tetindex];

	Matrix3d DmInv = t.Dm.inverse();

	Matrix3d F = Ds * DmInv;
	Matrix3d R, S;

	PolarDecompose::compute(F, R, S);
}

TetMesh::~TetMesh()
{
}
