#pragma once
#include "vector3d.h"
class Matrix4d
{
public:
	union{
		float mat[4][4];
		float arr[16];
	};
	Matrix4d(void);
	Matrix4d(float[4][4]);
	Matrix4d(float a00,float a01,float a02,float a03,
			 float a10,float a11,float a12,float a13,
			 float a20,float a21,float a22,float a23,
			 float a30,float a31,float a32,float a33);
	Matrix4d transpose();
	Matrix4d inverse();
	float determinant();
	Matrix4d operator*(Matrix4d&);
	vector3d operator*(vector3d&);
	Matrix4d operator+(Matrix4d&);
	Matrix4d operator-(Matrix4d&);

	~Matrix4d(void);
};

