#pragma once
#include "includes.h"
class bounds
{
public:
	virtual bool ptInBounds(vector3d& v)=0;
	virtual void translateBounds(vector3d& v)=0;
	virtual void scaleBounds(vector3d& v)=0;
	virtual void scaleBounds(vector3d& v, vector3d& center)=0;
	virtual void drawBounds()=0;
};

