#pragma once
#include "includes.h"
#include "quatn.h"

class GUIUtils
{
	vector3d cursorPos;
	vector3d sceneRot;
	bool mouseDown;
	bool rightMouseDown;
	int mouseX, mouseY;
	int pmouseX, pmouseY;

	inline
	void updateMouse(int x, int y)
	{
		pmouseX = mouseX;
		pmouseY = mouseY;
		mouseX = x;
		mouseY = y;
	}


public:
	GUIUtils() :mouseDown(false), rightMouseDown(false)
	{

	}

	void onMouseClick(int button, int state, int x, int y);
	void onMouseMove(int x, int y);
	void renderCursor();

	inline vector3d& getSceneRotation(){ return sceneRot; }
	inline vector3d& getCursorPos(){ return cursorPos; }


	static vector3d get3DFrom2D(float x, float y);

};