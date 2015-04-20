#pragma once
#include "PolarDecompose.h"
#include "Matrix3d.h"
#include "Node.h"

class Tet
{
	Node* nodes[4];
	vector3d restpos[4];

public:
	Tet(Node& n1, Node& n2, Node& n3, Node& n4)
	{
		nodes[0] = &n1;
		nodes[1] = &n2;
		nodes[2] = &n3;
		nodes[3] = &n4;

		restpos[0] = n1.position;
		restpos[1] = n2.position;
		restpos[2] = n3.position;
		restpos[3] = n4.position;
	}

	void getTransforms(Matrix3d& F, Matrix3d& R, Matrix3d& S)
	{
		Matrix3d Ds = Matrix3d(nodes[0]->position.x - nodes[3]->position.x, nodes[1]->position.x - nodes[3]->position.x, nodes[2]->position.x - nodes[3]->position.x,
							   nodes[0]->position.y - nodes[3]->position.y, nodes[1]->position.y - nodes[3]->position.y, nodes[2]->position.y - nodes[3]->position.y, 
							   nodes[0]->position.z - nodes[3]->position.z, nodes[1]->position.z - nodes[3]->position.z, nodes[2]->position.z - nodes[3]->position.z );


		Matrix3d Dm = Matrix3d(restpos[0].x - restpos[3].x, restpos[1].x - restpos[3].x, restpos[2].x - restpos[3].x,
							   restpos[0].y - restpos[3].y, restpos[1].y - restpos[3].y, restpos[2].y - restpos[3].y,
							   restpos[0].z - restpos[3].z, restpos[1].z - restpos[3].z, restpos[2].z - restpos[3].z).inverse();

		F = Ds * Dm;

		PolarDecompose::compute(F, R, S);

	}

	~Tet();
};

