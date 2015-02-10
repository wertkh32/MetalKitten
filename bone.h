#pragma once
#include "includes.h"
#include "ArticulatedDefines.h"
#include "quatn.h"
#include "Matrix3d.h"


class bone
{

protected:
	bone* children[MAX_CHILDREN];
	int no_children;
	vector3d position, endposition;
	quatn rotation;

	void	  rotateChild(quatn& q, vector3d& parentpos);
public:
	bone(vector3d& _pos, vector3d& _endpos):position(_pos),endposition(_endpos),no_children(0){}

	void addChild(bone* _bone);
	vector3d& getPosition(){return position;}
	vector3d& getEndPosition(){return endposition;}
	float	  getLength(){return (endposition - position).mag();}
	quatn&	  getRotation(){return rotation;}
	bool      boneFull(){return no_children == MAX_CHILDREN;}
	void      rotate(quatn& q);

	virtual void render()=0;

	~bone(void);
};

