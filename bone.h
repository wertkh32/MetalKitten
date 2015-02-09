#pragma once
#include "includes.h"
#include "quatn.h"

#define MAX_CHILDREN 3

class bone
{
	bone* children[MAX_CHILDREN];
	int no_children;
	vector3d position, endposition;
	quatn rotation;

public:
	bone(void);

	void addChild(bone* _bone);
	vector3d& getPosition(){return position;}
	vector3d& getEndPosition(){return endposition;}
	float	  getLength(){return (endposition - position).mag();}
	quatn&	  getRotation(){return rotation;}
	bool boneFull(){return no_children == MAX_CHILDREN;}


	~bone(void);
};

