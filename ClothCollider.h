#pragma once
#include "SpatialHash.h"
#include "MassSpringMesh.h"
#include "Collision.h"

#define MAX_TRIANGLES 300
#define EPSILON 0.01
#define SPHERE_RADIUS 0.3

class ClothCollider
{
	int n;
	SpatialHash hashtable;
	QuickArray<Triangle*, MAX_TRIANGLES> triangles;
	MassSpringMesh* mesh;
	float* forces;
public:
	ClothCollider(MassSpringMesh* _mesh);
	void initCollider();
	
	void addTriangle(int i0, int i1, int i2)
	{
		vector3d &v0 = mesh->getMassPoint(i0).vertex,
				 &v1 = mesh->getMassPoint(i1).vertex,
				 &v2 = mesh->getMassPoint(i2).vertex;
		Triangle* t = new Triangle(v0, v1, v2);
		triangles.push(t);
		hashtable.addGeometry(t);
	}
	float* applyCollisionConstraint(float* cur_state, float* prev_state, float* prev_prev_state);
	void render();

	~ClothCollider();
};

