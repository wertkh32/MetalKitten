#include "Matrix3d.h"


Matrix3d::Matrix3d(void)
{
	mat[0][0]=1;mat[0][1]=0;mat[0][2]=0;
	mat[1][0]=0;mat[1][1]=1;mat[1][2]=0;
	mat[2][0]=0;mat[2][1]=0;mat[2][2]=1;
}

Matrix3d::Matrix3d(const Matrix3d& mm)
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			mat[i][j]=mm.mat[i][j];
}

Matrix3d::Matrix3d(float m[3][3]){
for(int i=0;i<3;i++)
	for(int j=0;j<3;j++)
		mat[i][j]=m[i][j];
}

Matrix3d::Matrix3d(float a00,float a01,float a02,
				   float a10,float a11,float a12,
				   float a20,float a21,float a22)
{
	mat[0][0]=a00;mat[0][1]=a01;mat[0][2]=a02;
	mat[1][0]=a10;mat[1][1]=a11;mat[1][2]=a12;
	mat[2][0]=a20;mat[2][1]=a21;mat[2][2]=a22;
	
}


Matrix3d::Matrix3d(vector3d& c0, vector3d& c1, vector3d& c2)
{
	mat[0][0] = c0.x; mat[0][1] = c1.x; mat[0][2] = c2.x;
	mat[1][0] = c0.y; mat[1][1] = c1.y; mat[1][2] = c2.y;
	mat[2][0] = c0.z; mat[2][1] = c1.z; mat[2][2] = c2.z;
}

Matrix3d Matrix3d::skew(vector3d& v){
	return Matrix3d(0, -v.z, v.y,
					v.z, 0, -v.x,
					-v.y, v.x, 0);
}

Matrix3d Matrix3d::inverse()
{
	float d = determinant();

	return Matrix3d( (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])/d , -(mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1])/d ,  (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1])/d,
					-(mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])/d ,  (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0])/d , -(mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0])/d,
					 (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0])/d , -(mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0])/d ,  (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0])/d );
}

Matrix3d::~Matrix3d(void)
{
}
