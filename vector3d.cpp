#include "vector3d.h"

vector3d::vector3d(float _x, float _y, float _z)
{
	x=_x;
	y=_y;
	z=_z;
}

vector3d::vector3d(float v[3])
{
	x=v[0];
	y=v[1];
	z=v[2];
}

vector3d::vector3d(void){
	x=y=z=0;
}

vector3d::vector3d(const vector3d& v){
	x=v.x;
	y=v.y;
	z=v.z;
}


vector3d vector3d::lerp(vector3d& start, vector3d& end, float t){
	return (start*(1-t) + end*t);
}

//assumes start and end are unit vectors
vector3d vector3d::nlerp(vector3d& start, vector3d& end, float t){
	return lerp(start,end,t).unit();
}

//assumes start and end are unit vectors
vector3d vector3d::slerp(vector3d& start, vector3d& end, float t){
	float innerp = start.dot(end);
	innerp=CLAMP(innerp,-1,1);
	float angle= acos(innerp) * t;
	vector3d base = (end - start * innerp).unit();
	return start * cos(angle) + base * sin(angle);
}

vector3d::~vector3d(void){
}