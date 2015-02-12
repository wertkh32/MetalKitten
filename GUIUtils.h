#pragma once
#include "includes.h"
#include "quatn.h"

class GUIUtils
{
	vector3d cursorPos;
	quatn sceneRot;
	vector3d cameraPos;
	
	bool mouseDown;
	bool rightMouseDown;
	bool shiftDown;
	int mouseX, mouseY;
	int pmouseX, pmouseY;

	vector3d lookAt;

	inline
	void updateMouse(int x, int y)
	{
		pmouseX = mouseX;
		pmouseY = mouseY;
		mouseX = x;
		mouseY = y;
	}

	vector3d projectToCameraPlane(vector3d& p, float offset);


public:
	GUIUtils() :mouseDown(false), rightMouseDown(false),shiftDown(false), cameraPos(0.0, 0.0, 30.0), lookAt(0,0,-1)
	{

	}

	void onMouseClick(int button, int state, int x, int y);
	void onMouseMove(int x, int y);
	void onKeyPress(unsigned char key, int x, int y);
	void renderCursor();

	inline quatn& getSceneRotation(){ return sceneRot; }
	inline vector3d& getCameraPos(){ return cameraPos; }
	inline vector3d& getCursorPos(){ return cursorPos; }


	static vector3d get3DFrom2D(float x, float y);

};