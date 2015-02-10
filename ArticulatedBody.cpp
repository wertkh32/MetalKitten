#include "ArticulatedBody.h"



ArticulatedBody::~ArticulatedBody()
{
	for (int i = 0; i < bones.size(); i++)
		delete bones[i];
}
