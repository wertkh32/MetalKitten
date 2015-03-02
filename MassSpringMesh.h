#pragma once
#include "includes.h"

#define MAX_POINTS 100
#define MAX_SPRINGS ((MAX_POINTS * (MAX_POINTS - 1)) >> 1)

struct Spring
{
	int v0, v1;
	float restLength, stiffness;
	Spring(int _v0, int _v1, float _restLength, float _stiffness):v0(_v0), v1(_v1), restLength(_restLength), stiffness(_stiffness){}
};

struct MassPoint
{
	vector3d vertex;
	float mass;
	MassPoint(vector3d& v, float _mass):vertex(v),mass(_mass){}
};

class MassSpringMesh
{
	QuickArray<Spring, MAX_SPRINGS> springs;
	QuickArray<MassPoint, MAX_POINTS> masspoints;

public:
	MassSpringMesh();

	int getNoMassPoints() { return masspoints.size(); }
	int getNoSprings() { return springs.size(); }

	Spring& getSpring(int index){ return springs[index]; }
	MassPoint& getMassPoint(int index){ return masspoints[index]; }

	void addSpring(Spring& s){ springs.push(s); }
	void addMassPoint(MassPoint& m){ masspoints.push(m); }

	void getStiffnessWeightedLaplacian(float** A);
	void getJ(float** J);
	void getSystemMatrix(float** A);
	


	~MassSpringMesh();
};

