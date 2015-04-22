#pragma once
#include "includes.h"
#include "Node.h"
#include "PolarDecompose.h"
#include "Matrix3d.h"

#define MAX_NODES 1000
#define MAX_TETS 1000


struct Tet
{
	int node[4];
	float weight;
	float volume;
	Matrix3d Dm;
	Tet(){};
	Tet(int _n1, int _n2, int _n3, int _n4, float _weight, float _volume, Matrix3d& _Dm) : Dm(_Dm)
	{
		weight = _weight;
		volume = _volume;
		node[0] = _n1;
		node[1] = _n2;
		node[2] = _n3;
		node[3] = _n4;
	}
};

class TetMesh
{
	QuickArray<Node, MAX_NODES> nodes;
	QuickArray<vector3d, MAX_NODES> restpos;
	QuickArray<Tet, MAX_TETS> tets;

public:

	void addNode(Node& node)
	{
		nodes.push(node);
		restpos.push(node.position);
	}

	Matrix3d generateD(vector3d& v0, vector3d& v1, vector3d& v2, vector3d& v3)
	{
		return Matrix3d(v0.x - v3.x, v1.x - v3.x, v2.x - v3.x,
						v0.y - v3.y, v1.y - v3.y, v2.y - v3.y,
						v0.z - v3.z, v1.z - v3.z, v2.z - v3.z);
	}


	void addTet(int t1, int t2, int t3, int t4, float weight)
	{
		Matrix3d Dm = Matrix3d(restpos[t1].x - restpos[t4].x, restpos[t2].x - restpos[t4].x, restpos[t3].x - restpos[t4].x,
							   restpos[t1].y - restpos[t4].y, restpos[t2].y - restpos[t4].y, restpos[t3].y - restpos[t4].y,
							   restpos[t1].z - restpos[t4].z, restpos[t2].z - restpos[t4].z, restpos[t3].z - restpos[t4].z);

		float vol = 1.0 / 6.0 * fabs(Dm.determinant());

		tets.push(Tet(t1, t2, t3, t4, weight,vol,Dm));
	}

	void addTet(int t[4], float weight)
	{
		Matrix3d Dm = Matrix3d(restpos[t[0]].x - restpos[t[3]].x, restpos[t[1]].x - restpos[t[3]].x, restpos[t[2]].x - restpos[t[3]].x,
							   restpos[t[0]].y - restpos[t[3]].y, restpos[t[1]].y - restpos[t[3]].y, restpos[t[2]].y - restpos[t[3]].y,
							   restpos[t[0]].z - restpos[t[3]].z, restpos[t[1]].z - restpos[t[3]].z, restpos[t[2]].z - restpos[t[3]].z);

		float vol = 1.0 / 6.0 * fabs(Dm.determinant());

		tets.push(Tet(t[0], t[1], t[2], t[3], weight, vol, Dm));
	}

	Node& getNode(int index)
	{
		return nodes[index]; 
	}

	Tet& getTet(int index)
	{
		return tets[index];
	}

	vector3d& getRestPosition(int index)
	{
		return restpos[index];
	}

	int getNumNodes()
	{
		return nodes.size();
	}

	int getNumTets()
	{
		return tets.size();
	}

	void getTransforms(int tetindex, Matrix3d& Ds, Matrix3d& F, Matrix3d& R, Matrix3d& S);

	Matrix3d getRotation(int tetindex, Matrix3d& Ds);

	void renderTet(int tetindex);
	void renderMesh();

	TetMesh();
	~TetMesh();
};

