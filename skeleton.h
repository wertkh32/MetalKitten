#pragma once
#include "includes.h"
#include "boundsAABB.h"
#include "quatn.h"
#include "Matrix4d.h"

class skeleton
{
	quatn id;
	
public:
	boundsAABB* bb;
	char* tag;
	skeleton* child;
	int child_no;

	//float length;
	vector3d startpt, endpt;
	QuickArray<pair<vector3d*,float>> vertices;

	quatn rot;

	void setAABB(boundsAABB* bbb);
	void addVertex(vector3d& v,float weight);
	void makeChild(int child_no);
	void addRotation(quatn& q);
	void rotateAllVertices(vector3d&, Matrix4d& rotation);
	void rotateAllVertices();
	void rotate(vector3d& v, vector3d& center,Matrix4d& rots,float weight);
	void rotate(vector3d& v, vector3d& center,Matrix4d& rots);
	skeleton(void);
	~skeleton(void);
};

inline
void skeleton::rotate(vector3d& v,vector3d& center,Matrix4d& mrots,float weight){
	v=v*(1-weight)+((mrots*(v-center))+center)*weight;
}

inline
void skeleton::rotate(vector3d& v,vector3d& center,Matrix4d& mrots){
	v=(mrots*(v-center))+center;
}

