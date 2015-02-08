#include "GUIUtils.h"


vector3d  GUIUtils::get3DFrom2D(float x, float y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glPushMatrix();

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = x;
	winY = viewport[3] - y;
	glReadPixels(x, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	glPopMatrix();

	return vector3d (posX, posY, posZ);

}

vector3d GUIUtils::projectToCameraPlane(vector3d& p, float offset)
{
	vector3d dir = p.unit();
	float t = (offset - lookAt.dot(dir));
	return dir * t;
}

//render after everything else
void GUIUtils::renderCursor()
{
	if (!rightMouseDown && mouseDown)
	{
	
		cursorPos = get3DFrom2D(mouseX, mouseY);
		vector3d ppp = sceneRot.conjugate() * cursorPos - cameraPos;
		cursorPos = projectToCameraPlane(ppp, 20);
		cursorPos = sceneRot * (cursorPos + cameraPos);
	}
	//printf("%f %f %f\n", ppp.x, ppp.y, ppp.z);
	printf("%f %f %f\n", cursorPos.x, cursorPos.y, cursorPos.z);

	glPushMatrix();
	glTranslatef(cursorPos.x, cursorPos.y, cursorPos.z);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();
}

void GUIUtils::onMouseClick(int button, int state, int x, int y)
{
	updateMouse(x, y);
	mouseDown = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
	rightMouseDown = (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN);
}

void GUIUtils::onMouseMove(int x, int y)
{
	if (mouseDown)
	{
		updateMouse(x, y);
	}
	else if (rightMouseDown)
	{
		updateMouse(x, y);
		int sgn = (mouseX - pmouseX) < 0 ? -1 : 1;
		sceneRot *= quatn(vector3d(0, 1, 0), sgn);
	}
}

void GUIUtils::onKeyPress(unsigned char key, int x, int y)
{

}