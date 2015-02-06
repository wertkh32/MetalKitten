#pragma once
#include "includes.h"
#include "skeleton.h"

class model
{
	QuickArray<vector3d> vertices;
	QuickArray<vector3d> normals;
	//vector<vector3d> original;
	skeleton skel;
	
public:
	skeleton *hip,
			 *torso,
			 *head,
			 *larm,
			 *rarm,
			 *lfarm,
			 *rfarm,
			 *lleg,
			 *rleg,
			 *lcalf,
			 *rcalf;



	model(char* filename);
	~model(void);

	void reloadmodel();
	void addrotation(skeleton* part, float x_angle, float y_angle, float z_angle);
	void draw();
};

