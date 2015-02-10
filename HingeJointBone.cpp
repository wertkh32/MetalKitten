#include "HingeJointBone.h"

void HingeJointBone::rotate(float angle)
{
	bone::rotate(quatn((rotation * axis),angle));
}

vector3d HingeJointBone::getJacobianRow(vector3d& endeffector)
{
	vector3d a = endeffector - position;
	
	return (rotation * axis).cross(a);
}

void HingeJointBone::render()
{
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glutSolidCube(1);
	glPopMatrix();

	glLineWidth(10);
	glPushMatrix();
		glBegin(GL_LINES);
		glVertex3fv(position.coords);
		glVertex3fv(endposition.coords);
		glEnd();

		glBegin(GL_LINES);
		glVertex3fv(position.coords);
		glVertex3fv((position+ (rotation * axis) * 2).coords);
		glEnd();

	glPopMatrix();
}

HingeJointBone::~HingeJointBone(void)
{
}
