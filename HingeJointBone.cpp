#include "HingeJointBone.h"

void HingeJointBone::rotate(float angle)
{
	//rotate(quatn((rotation * axis),angle));
	quatn q = quatn((rotation * axis),angle);
	rotation = q * rotation; 
	endposition = q * (endposition - position) + position;

	for (int i = 0; i < no_children; i++)
		children[i]->rotateChild(q,endposition);
}

vector3d HingeJointBone::getJacobianRow(vector3d& endeffector)
{
	vector3d a = endeffector - position;
	
	return (rotation * axis).cross(a);
}

void HingeJointBone::rotate(quatn& p)
{
	quatn q = applyConstraints(p);
	rotation = q * rotation; 
	endposition = q * (endposition - position) + position;

	for (int i = 0; i < no_children; i++)
		children[i]->rotateChild(q,endposition);
}

void HingeJointBone::render()
{
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glutSolidCube(1);
	glPopMatrix();

	glLineWidth(10);
	glPushMatrix();
		glColor3f(1.0,0,0);
		glBegin(GL_LINES);
		glVertex3fv(position.coords);
		glVertex3fv(endposition.coords);
		glEnd();

		glColor3f(0,1.0,0);
		glBegin(GL_LINES);
		glVertex3fv(position.coords);
		glVertex3fv((position+ (rotation * axis) * 2).coords);
		glEnd();

	glPopMatrix();
}

HingeJointBone::~HingeJointBone(void)
{
}
