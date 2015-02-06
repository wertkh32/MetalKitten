#pragma once
#include "includes.h"

class vector3d
{
public:
	union{
		struct{float x,y,z;};
		struct{float coords[3];}; //ensure compatability with OpenGL
	};
	vector3d(float, float, float);
	vector3d(float[3]);
	vector3d(const vector3d& v);
	vector3d(void);
	~vector3d(void);


	float    dot(vector3d& v);
	vector3d cross(vector3d& v);
	vector3d unit();
	float    mag();

	vector3d& operator=(vector3d&); //ensure well defined assignment
	vector3d operator*(float);		//scalar mul
	vector3d operator*(vector3d& v);//scalar mul
	vector3d operator/(float);		//scalar divide
	vector3d operator+(vector3d& v);//add
	vector3d operator-(vector3d& v);//subtract
	vector3d operator-();			 //inverse
	vector3d operator~();			 //unit

	static vector3d lerp(vector3d& start, vector3d& end, float t);
	static vector3d nlerp(vector3d& start, vector3d& end, float t);
	static vector3d slerp(vector3d& start, vector3d& end, float t);
};

inline
vector3d& vector3d::operator=(vector3d& v){
	x=v.x;
	y=v.y;
	z=v.z;
	return *this;
}

inline
float vector3d::dot(vector3d& v){
	return x * v.x + y * v.y + z * v.z;
}

inline
vector3d vector3d::cross(vector3d& v){
	return vector3d(y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y);
}

inline
vector3d vector3d::unit(){
float m=mag();
return vector3d(x/m, y/m, z/m);
}

inline
vector3d vector3d::operator~(){
	return unit();
}

inline
vector3d vector3d::operator*(float k){
	return vector3d(x*k,y*k,z*k);
}

inline
vector3d vector3d::operator*(vector3d& v){
	return vector3d(x*v.x,y*v.y,z*v.z);
}

inline
vector3d vector3d::operator/(float k){
	return vector3d(x/k,y/k,z/k);
}

inline
vector3d vector3d::operator+(vector3d& v){
	return vector3d(x+v.x,y+v.y,z+v.z);
}

inline
vector3d vector3d::operator-(vector3d& v){
	return vector3d(x-v.x,y-v.y,z-v.z);
}

inline
vector3d vector3d::operator-(){
	return vector3d(-x, -y, -z);
}

inline
float vector3d::mag(){
	return sqrt(x*x+y*y+z*z);
}