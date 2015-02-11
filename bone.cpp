#include "bone.h"

void bone::addChild(bone* _bone)
{
	if(!boneFull())
		children[no_children++] = _bone;
}

void bone::rotate(quatn& p)
{
	quatn q = applyConstraints(p);
	rotation = q * rotation; 
	endposition = q * (endposition - position) + position;

	for (int i = 0; i < no_children; i++)
		children[i]->rotateChild(q,endposition);
}

void bone::rotateChild(quatn& q, vector3d& parentendpos)
{
	rotation = q * rotation;
	endposition = q * (endposition - position) + parentendpos;
	position = parentendpos;

	for (int i = 0; i < no_children; i++)
		children[i]->rotateChild(q, endposition);
}

bone::~bone(void)
{
}
