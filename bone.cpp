#include "bone.h"


bone::bone(void)
{
}


void bone::addChild(bone* _bone)
{
	if(!boneFull())
		children[no_children++] = _bone;
}

bone::~bone(void)
{
}
