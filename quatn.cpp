#include "quatn.h"


quatn::quatn(void)
{
	s=1;
	v.x=v.y=v.z=0;

}

quatn::quatn(float sca, vector3d vec){
	s=sca;
	v=vec;
}

quatn::quatn(vector3d q, float angle)
{
	angle=TO_RAD(angle);
	s=cos(angle/2);
	v=q*sin(angle/2);
}

quatn::quatn(float x, float y, float z){
	x=TO_RAD(x);
	y=TO_RAD(y);
	z=TO_RAD(z);
	float cosx=cos(x*0.5),
		  cosy=cos(y*0.5),
		  cosz=cos(z*0.5);

	float sinx=sin(x*0.5),
		  siny=sin(y*0.5),
		  sinz=sin(z*0.5);

	s= cosz*cosy*cosx + sinz*siny*sinx;
	v=vector3d(cosz*cosy*sinx - sinz*siny*cosx,
			   cosz*siny*cosx + sinz*cosy*sinx,
			   sinz*cosy*cosx - cosz*siny*sinx);
}

Matrix4d quatn::toRotMatrix(){
	float x2 = v.x * v.x;
	float y2 = v.y * v.y;
	float z2 = v.z * v.z;
	float xy = v.x * v.y;
	float xz = v.x * v.z;
	float yz = v.y * v.z;
	float wx = s * v.x;
	float wy = s * v.y;
	float wz = s * v.z;
 
	return Matrix4d( 1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
			2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix3d quatn::toRotMatrix3d(){
	float x2 = v.x * v.x;
	float y2 = v.y * v.y;
	float z2 = v.z * v.z;
	float xy = v.x * v.y;
	float xz = v.x * v.z;
	float yz = v.y * v.z;
	float wx = s * v.x;
	float wy = s * v.y;
	float wz = s * v.z;

	return Matrix3d(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy),
		2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 
		2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2));
}

quatn::~quatn(void)
{
}
