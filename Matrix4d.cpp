#include "Matrix4d.h"


Matrix4d::Matrix4d(void)
{
	mat[0][0]=1;mat[0][1]=0;mat[0][2]=0;mat[0][3]=0;
	mat[1][0]=0;mat[1][1]=1;mat[1][2]=0;mat[1][3]=0;
	mat[2][0]=0;mat[2][1]=0;mat[2][2]=1;mat[2][3]=0;
	mat[3][0]=0;mat[3][1]=0;mat[3][2]=0;mat[3][3]=1;
}

Matrix4d::Matrix4d(float m[4][4]){
for(int i=0;i<4;i++)
	for(int j=0;j<4;j++)
		mat[i][j]=m[i][j];
}

Matrix4d::Matrix4d(float a00,float a01,float a02,float a03,
				   float a10,float a11,float a12,float a13,
				   float a20,float a21,float a22,float a23,
				   float a30,float a31,float a32,float a33)
{
	mat[0][0]=a00;mat[0][1]=a01;mat[0][2]=a02;mat[0][3]=a03;
	mat[1][0]=a10;mat[1][1]=a11;mat[1][2]=a12;mat[1][3]=a13;
	mat[2][0]=a20;mat[2][1]=a21;mat[2][2]=a22;mat[2][3]=a23;
	mat[3][0]=a30;mat[3][1]=a31;mat[3][2]=a32;mat[3][3]=a33;
}

Matrix4d Matrix4d::transpose(){
return Matrix4d(mat[0][0],mat[1][0],mat[2][0],mat[3][0],
				mat[0][1],mat[1][1],mat[2][1],mat[3][1],
				mat[0][2],mat[1][2],mat[2][2],mat[3][2],
				mat[0][3],mat[1][3],mat[2][3],mat[3][3]);
}


float Matrix4d::determinant()
{
	return (mat[0][0] * (-mat[2][3] * mat[3][2] * mat[1][1] + mat[2][2] * mat[3][3] * mat[1][1] + mat[2][3] * mat[3][1] * mat[1][2] - mat[2][2] * mat[3][1] * mat[1][3] - mat[3][3] * mat[1][2] * mat[2][1] + mat[3][2] * mat[1][3] * mat[2][1]) +
		mat[0][1] * (mat[2][3] * mat[3][2] * mat[1][0] - mat[2][2] * mat[3][3] * mat[1][0] - mat[2][3] * mat[3][0] * mat[1][2] + mat[2][2] * mat[3][0] * mat[1][3] + mat[3][3] * mat[1][2] * mat[2][0] - mat[3][2] * mat[1][3] * mat[2][0]) +
		mat[0][2] * (-mat[2][3] * mat[3][1] * mat[1][0] + mat[2][3] * mat[3][0] * mat[1][1] - mat[3][3] * mat[1][1] * mat[2][0] + mat[3][1] * mat[1][3] * mat[2][0] + mat[3][3] * mat[1][0] * mat[2][1] - mat[3][0] * mat[1][3] * mat[2][1]) +
		mat[0][3] * (mat[2][2] * mat[3][1] * mat[1][0] - mat[2][2] * mat[3][0] * mat[1][1] + mat[3][2] * mat[1][1] * mat[2][0] - mat[3][1] * mat[1][2] * mat[2][0] - mat[3][2] * mat[1][0] * mat[2][1] + mat[3][0] * mat[1][2] * mat[2][1]));
}

Matrix4d Matrix4d::inverse()
{
	//from Mathematica
	float result[4][4];
	result[0][0] = -mat[2][3] * mat[3][2] * mat[1][1] + mat[2][2] * mat[3][3] * mat[1][1] + mat[2][3] * mat[3][1] * mat[1][2] - mat[2][2] * mat[3][1] * mat[1][3] - mat[3][3] * mat[1][2] * mat[2][1] + mat[3][2] * mat[1][3] * mat[2][1];
	result[0][1] = mat[0][1] * mat[2][3] * mat[3][2] - mat[0][1] * mat[2][2] * mat[3][3] - mat[2][3] * mat[3][1] * mat[0][2] + mat[2][2] * mat[3][1] * mat[0][3] + mat[3][3] * mat[0][2] * mat[2][1] - mat[3][2] * mat[0][3] * mat[2][1];
	result[0][2] = -mat[3][3] * mat[0][2] * mat[1][1] + mat[3][2] * mat[0][3] * mat[1][1] + mat[0][1] * mat[3][3] * mat[1][2] - mat[3][1] * mat[0][3] * mat[1][2] - mat[0][1] * mat[3][2] * mat[1][3] + mat[3][1] * mat[0][2] * mat[1][3];
	result[0][3] = mat[2][3] * mat[0][2] * mat[1][1] - mat[2][2] * mat[0][3] * mat[1][1] - mat[0][1] * mat[2][3] * mat[1][2] + mat[0][1] * mat[2][2] * mat[1][3] + mat[0][3] * mat[1][2] * mat[2][1] - mat[0][2] * mat[1][3] * mat[2][1];
	result[1][0] = mat[2][3] * mat[3][2] * mat[1][0] - mat[2][2] * mat[3][3] * mat[1][0] - mat[2][3] * mat[3][0] * mat[1][2] + mat[2][2] * mat[3][0] * mat[1][3] + mat[3][3] * mat[1][2] * mat[2][0] - mat[3][2] * mat[1][3] * mat[2][0];
	result[1][1] = -mat[0][0] * mat[2][3] * mat[3][2] + mat[0][0] * mat[2][2] * mat[3][3] + mat[2][3] * mat[3][0] * mat[0][2] - mat[2][2] * mat[3][0] * mat[0][3] - mat[3][3] * mat[0][2] * mat[2][0] + mat[3][2] * mat[0][3] * mat[2][0];
	result[1][2] = mat[3][3] * mat[0][2] * mat[1][0] - mat[3][2] * mat[0][3] * mat[1][0] - mat[0][0] * mat[3][3] * mat[1][2] + mat[3][0] * mat[0][3] * mat[1][2] + mat[0][0] * mat[3][2] * mat[1][3] - mat[3][0] * mat[0][2] * mat[1][3];
	result[1][3] = -mat[2][3] * mat[0][2] * mat[1][0] + mat[2][2] * mat[0][3] * mat[1][0] + mat[0][0] * mat[2][3] * mat[1][2] - mat[0][0] * mat[2][2] * mat[1][3] - mat[0][3] * mat[1][2] * mat[2][0] + mat[0][2] * mat[1][3] * mat[2][0];
	result[2][0] = -mat[2][3] * mat[3][1] * mat[1][0] + mat[2][3] * mat[3][0] * mat[1][1] - mat[3][3] * mat[1][1] * mat[2][0] + mat[3][1] * mat[1][3] * mat[2][0] + mat[3][3] * mat[1][0] * mat[2][1] - mat[3][0] * mat[1][3] * mat[2][1];
	result[2][1] = -mat[0][1] * mat[2][3] * mat[3][0] + mat[0][0] * mat[2][3] * mat[3][1] + mat[0][1] * mat[3][3] * mat[2][0] - mat[3][1] * mat[0][3] * mat[2][0] - mat[0][0] * mat[3][3] * mat[2][1] + mat[3][0] * mat[0][3] * mat[2][1];
	result[2][2] = -mat[0][1] * mat[3][3] * mat[1][0] + mat[3][1] * mat[0][3] * mat[1][0] + mat[0][0] * mat[3][3] * mat[1][1] - mat[3][0] * mat[0][3] * mat[1][1] + mat[0][1] * mat[3][0] * mat[1][3] - mat[0][0] * mat[3][1] * mat[1][3];
	result[2][3] = mat[0][1] * mat[2][3] * mat[1][0] - mat[0][0] * mat[2][3] * mat[1][1] + mat[0][3] * mat[1][1] * mat[2][0] - mat[0][1] * mat[1][3] * mat[2][0] - mat[0][3] * mat[1][0] * mat[2][1] + mat[0][0] * mat[1][3] * mat[2][1];
	result[3][0] = mat[2][2] * mat[3][1] * mat[1][0] - mat[2][2] * mat[3][0] * mat[1][1] + mat[3][2] * mat[1][1] * mat[2][0] - mat[3][1] * mat[1][2] * mat[2][0] - mat[3][2] * mat[1][0] * mat[2][1] + mat[3][0] * mat[1][2] * mat[2][1];
	result[3][1] = mat[0][1] * mat[2][2] * mat[3][0] - mat[0][0] * mat[2][2] * mat[3][1] - mat[0][1] * mat[3][2] * mat[2][0] + mat[3][1] * mat[0][2] * mat[2][0] + mat[0][0] * mat[3][2] * mat[2][1] - mat[3][0] * mat[0][2] * mat[2][1];
	result[3][2] = mat[0][1] * mat[3][2] * mat[1][0] - mat[3][1] * mat[0][2] * mat[1][0] - mat[0][0] * mat[3][2] * mat[1][1] + mat[3][0] * mat[0][2] * mat[1][1] - mat[0][1] * mat[3][0] * mat[1][2] + mat[0][0] * mat[3][1] * mat[1][2];
	result[3][3] = -mat[0][1] * mat[2][2] * mat[1][0] + mat[0][0] * mat[2][2] * mat[1][1] - mat[0][2] * mat[1][1] * mat[2][0] + mat[0][1] * mat[1][2] * mat[2][0] + mat[0][2] * mat[1][0] * mat[2][1] - mat[0][0] * mat[1][2] * mat[2][1];
	float d = 1.0 / (mat[0][0] * result[0][0] + mat[0][1] * result[1][0] + mat[0][2] * result[2][0] + mat[0][3] * result[3][0]);
	for (int i = 0; i<4; i++)
	for (int j = 0; j<4; j++)
		result[i][j] *= d;
	return Matrix4d(result);
}


Matrix4d Matrix4d::operator*(Matrix4d& mm){
	float m[4][4]={0};
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			for(int k=0;k<4;k++)
				m[i][j]+= mat[i][k]*mm.mat[k][j];
	return Matrix4d(m);
}

vector3d Matrix4d::operator*(vector3d& vv){
	float v[3]={0};
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			v[i]+=mat[i][j] * vv.coords[j];
	return vector3d(v);
}

Matrix4d Matrix4d::operator+(Matrix4d& mm){
	float m[4][4];
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			m[i][j] = mat[i][j]+mm.mat[i][j];
	return Matrix4d(m);
}

Matrix4d Matrix4d::operator-(Matrix4d& mm){
	float m[4][4];
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			m[i][j] = mat[i][j]-mm.mat[i][j];
	return Matrix4d(m);
}

Matrix4d::~Matrix4d(void)
{
}
