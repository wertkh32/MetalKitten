#pragma once
#include "defines.h"
#include "bounds.h"
class boundsAABB : public virtual bounds
{
	vector3d a,b;
public:
	boundsAABB(float _x1,float _y1,float _z1,float _x2,float _y2,float _z2);
	boundsAABB(float _x1,float _y1,float _x2,float _y2);
	~boundsAABB(void);
	bool ptInBounds(vector3d& v);
	void translateBounds(vector3d& v);
	void scaleBounds(vector3d& v);
	void scaleBounds(vector3d& v, vector3d& center);
	void drawBounds();
};

