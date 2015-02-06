#include "boundsAABB.h"


boundsAABB::boundsAABB(float _x1,float _y1,float _z1,float _x2,float _y2,float _z2)
{
	a.x=_x1;
	a.y=_y1;
	a.z=_z1;
	b.x=_x2;
	b.y=_y2;
	b.z=_z2;
}

boundsAABB::boundsAABB(float _x1,float _y1,float _x2,float _y2){
	a.x=_x1;
	a.y=_y1;
	a.z=-INF;
	b.x=_x2;
	b.y=_y2;
	b.z=INF;
}

bool boundsAABB::ptInBounds(vector3d& v){
	if(v.x>a.x && v.x<b.x &&
	   v.y>a.y && v.y<b.y &&
	   v.z>a.z && v.z<b.z)
	   return true;

	return false;
}

void boundsAABB::translateBounds(vector3d& v){
	a = a+v;
	b = b+v;
}

void boundsAABB::scaleBounds(vector3d& v){
	a = a * v;
	b = b * v;
}

void boundsAABB::scaleBounds(vector3d& v, vector3d& center){
	a = (a-center) * v + center;
	b = (b-center) * v + center;
}

void boundsAABB::drawBounds(){
glPushMatrix();
glColor3f(0,1,0);
glLineWidth(2);
glBegin(GL_LINES);
//top rect
glVertex3f(a.x,a.y,a.z); 
glVertex3f(b.x,a.y,a.z);

glVertex3f(a.x,a.y,b.z);
glVertex3f(b.x,a.y,b.z);

glVertex3f(a.x,a.y,a.z);
glVertex3f(a.x,a.y,b.z);

glVertex3f(b.x,a.y,a.z);
glVertex3f(b.x,a.y,b.z);

//bot rect
glVertex3f(a.x,b.y,a.z);
glVertex3f(b.x,b.y,a.z);

glVertex3f(a.x,b.y,b.z);
glVertex3f(b.x,b.y,b.z);

glVertex3f(a.x,b.y,a.z);
glVertex3f(a.x,b.y,b.z);

glVertex3f(b.x,b.y,a.z);
glVertex3f(b.x,b.y,b.z);

//4 mid lines
glVertex3f(a.x,a.y,a.z); 
glVertex3f(a.x,b.y,a.z);

glVertex3f(a.x,a.y,b.z); 
glVertex3f(a.x,b.y,b.z);

glVertex3f(b.x,a.y,a.z); 
glVertex3f(b.x,b.y,a.z);

glVertex3f(b.x,a.y,b.z); 
glVertex3f(b.x,b.y,b.z);

glEnd();
glPopMatrix();
}

boundsAABB::~boundsAABB(void)
{
}
