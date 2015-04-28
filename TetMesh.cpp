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

	

	Matrix3d F = Ds * t.Bm;
	Matrix3d R, S;

	PolarDecompose::compute(F, R, S);

	if(R.determinant() < 0)
		R = Matrix3d(-1, 0, 0,
					 0, -1, 0,
					 0, 0, -1) * R;

	return R;
}

void
TetMesh::renderTet(int tetindex)
{
	Tet& t = tets[tetindex];

	int i0 = t.node[0];
	int i1 = t.node[1];
	int i2 = t.node[2];
	int i3 = t.node[3];

	glPushMatrix();

	glLineWidth(3.0);
	glBegin(GL_LINES);
	glVertex3fv(nodes[i3].position.coords);
	glVertex3fv(nodes[i0].position.coords);

	glVertex3fv(nodes[i3].position.coords);
	glVertex3fv(nodes[i1].position.coords);

	glVertex3fv(nodes[i3].position.coords);
	glVertex3fv(nodes[i2].position.coords);


	glVertex3fv(nodes[i0].position.coords);
	glVertex3fv(nodes[i1].position.coords);

	glVertex3fv(nodes[i1].position.coords);
	glVertex3fv(nodes[i2].position.coords);

	glVertex3fv(nodes[i2].position.coords);
	glVertex3fv(nodes[i0].position.coords);

	glEnd();


	glPopMatrix();
}

void
TetMesh::renderMesh()
{
	for (int i = 0; i < getNumTets(); i++)
	{
		renderTet(i);
	}
}


TetMesh::~TetMesh()
{
}
