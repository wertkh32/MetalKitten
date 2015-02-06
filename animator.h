#pragma once
#include "includes.h"
#include "skeleton.h"

class animator
{
	skeleton* target;
	quatn framerot;
	int frames, currframe;
public:
	animator(skeleton* s,float x, float y, float z, int frame_num);
	bool animate();
	void reverse();
	~animator(void);
};

