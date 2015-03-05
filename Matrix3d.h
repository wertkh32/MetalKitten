#pragma once
#include "vector3d.h"

class Matrix3d
{
public:
	union{
		float mat[3][3];
		float arr[9];
		struct{ vector3d rows[3]; };
		};
	Matrix3d(void);
	Matrix3d(float[3][3]);
	Matrix3d(const Matrix3d& m);
	Matrix3d(float a00,float a01,float a02,
			 float a10,float a11,float a12,
			 float a20,float a21,float a22);
	Matrix3d(vector3d& c0, vector3d& c1, vector3d& c2);
	inline Matrix3d transpose();
	inline float determinant();
	inline Matrix3d operator*(const Matrix3d&);
	inline Matrix3d operator*(float);
	inline vector3d operator*(const vector3d&);
	inline Matrix3d operator+(const Matrix3d&);
	inline Matrix3d operator-(const Matrix3d&);
	float& operator()(int i,int j){return mat[i][j];}
	vector3d& operator()(int i){ return rows[i]; }
	float trace(){return mat[0][0] + mat[1][1] + mat[2][2];}

	Matrix3d inverse();
	static Matrix3d skew(vector3d& v);
	void print(){for(int i=0;i<3;i++,printf("\n"))for(int j=0;j<3;j++)printf("%f ",mat[i][j]);}

	~Matrix3d(void);
};

inline
float Matrix3d::determinant()
{
	return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) 
		 - mat[0][1] * (mat[2][2] * mat[1][0] - mat[1][2] * mat[2][0]) 
		 + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
}

inline
Matrix3d Matrix3d::transpose(){
return Matrix3d(mat[0][0],mat[1][0],mat[2][0],
				mat[0][1],mat[1][1],mat[2][1],
				mat[0][2],mat[1][2],mat[2][2]);
}

inline
Matrix3d Matrix3d::operator*(const Matrix3d& mm){
	float m[3][3]={0};
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			for(int k=0;k<3;k++)
				m[i][j]+= mat[i][k]*mm.mat[k][j];
	return Matrix3d(m);
}

inline
vector3d Matrix3d::operator*(const vector3d& vv){
	float v[3]={0};
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			v[i]+=mat[i][j] * vv.coords[j];
	return vector3d(v);
}

inline
Matrix3d Matrix3d::operator*(float k){
	float m[3][3] = {0};
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
				m[i][j] = mat[i][j] * k;
	return Matrix3d(m);
}

inline
Matrix3d Matrix3d::operator+(const Matrix3d& mm){
	float m[3][3];
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			m[i][j] = mat[i][j]+mm.mat[i][j];
	return Matrix3d(m);
}

inline
Matrix3d Matrix3d::operator-(const Matrix3d& mm){
	float m[3][3];
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			m[i][j] = mat[i][j]-mm.mat[i][j];
	return Matrix3d(m);
}

