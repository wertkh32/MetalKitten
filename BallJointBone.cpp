#include "BallJointBone.h"

Matrix3d BallJointBone::getJacobian3DOF(vector3d& endeffector)
{
	vector3d a = endeffector - position;
	Matrix3d jacobian;
	jacobian.rows[0] = (rotation * vector3d(1, 0, 0)).cross(a);
	jacobian.rows[1] = (rotation * vector3d(0, 1, 0)).cross(a);
	jacobian.rows[2] = (rotation * vector3d(0, 0, 1)).cross(a);
	
	return jacobian;
}

vector3d BallJointBone::getJacobianQuatn(vector3d& endeffector, vector3d& goal)
{
	vector3d a = endeffector - position;
	vector3d axis = (a.cross(goal - position)).unit();

	return axis.cross(a);
}


void BallJointBone::rotate(vector3d& angles)
{
	quatn xrot = quatn(vector3d(1, 0, 0), angles.x);
	quatn yrot = quatn(vector3d(0, 1, 0), angles.y);
	quatn zrot = quatn(vector3d(0, 0, 1), angles.z);

	quatn T = zrot * yrot * xrot;

	bone::rotate(T);
}

void BallJointBone::render()
{
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(1, 10, 10);
	glPopMatrix();
	
	glPushMatrix();
	glLineWidth(10);
		glBegin(GL_LINES);
		glVertex3fv(position.coords);
		glVertex3fv(endposition.coords);
		glEnd();
	glPopMatrix();
}

BallJointBone::~BallJointBone(void)
{
}
