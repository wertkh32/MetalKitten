#pragma once
#include "vector3d.h"
#include "Matrix4d.h"

class quatn
{
public:
	float s;
	vector3d v;
	quatn(void);
	quatn(float, vector3d);

	quatn(vector3d v, float angle);
	quatn(float x, float y, float z);

	//vector3d rotate(vector3d& v);

	quatn conjugate();
	quatn inverse();
	quatn unit();
	float mag();

	quatn operator+(quatn& q);//add
	quatn operator-(quatn& q);//subtract 
	quatn operator*(quatn& q);//multiply
	vector3d operator*(vector3d& vec);//multiply vec
	quatn operator*(float k);//multiply
	quatn& operator*=(quatn& q);//multiply(=)

	quatn lerp(quatn& end, float t);
	Matrix4d toRotMatrix();
	~quatn(void);
};

inline
quatn quatn::conjugate(){
return quatn(s,-v);
}

inline
quatn quatn::inverse(){
	float m=mag();
	return quatn(s/m, vector3d(-v.x/m, -v.y/m, -v.z/m));
}

inline
quatn quatn::unit(){
	float m=mag();
	return quatn(s/m, vector3d(v.x/m, v.y/m, v.z/m));
}

inline
float quatn::mag(){
return sqrt(s*s + v.x*v.x + v.y*v.y + v.z*v.z);
}

inline
quatn quatn::operator+(quatn& q){
	return quatn(s+q.s, vector3d(v.x+q.v.x, v.y+q.v.y, v.z+q.v.z));
}

inline
quatn quatn::operator-(quatn& q){
	return quatn(s-q.s, vector3d(v.x-q.v.x, v.y-q.v.y, v.z-q.v.z));
}

inline
quatn quatn::operator*(quatn& q){
	return quatn(s*q.s - v.dot(q.v), vector3d( q.v*s + v*q.s + v.cross(q.v) ) );
}

inline
quatn quatn::operator*(float k){
	return quatn(s*k,vector3d(v.x*k,v.y*k,v.z*k));
}

inline
quatn& quatn::operator*=(quatn& q){
	return *this = ((*this) * q);
}

inline
quatn quatn::lerp(quatn& dest, float t){
return ((*this)*(1-t) + dest*t).unit();
}

inline
vector3d quatn::operator*(vector3d& vec)
{
	vector3d t = v.cross(vec) * 2;
	return vec + t * s + v.cross(t);
}

/*
float rotcomp1;
vector3d rotcomp2, rotcomp3;
//	rotcomp1=((s * s) - (v.dot(v)));
//	rotcomp2=v * 2;
//	rotcomp3=v * 2 * s;
inline
vector3d quatn::rotate(vector3d& r){
	return r * rotcomp1 + rotcomp2 * (v).dot(r) + rotcomp3.cross(r);
}*/