#include "animator.h"


animator::animator(skeleton* s,float x, float y, float z, int frame_num)
{
	target=s;
	target->rot=quatn(x/frame_num,y/frame_num,z/frame_num);
	frames=frame_num;
	currframe=0;

}

bool animator::animate(){
if(currframe<frames){
++currframe;
target->rotateAllVertices();
//printf("rotating... %d\n", currframe);
return true;
}
return false;
}

void animator::reverse(){
target->rot=target->rot.inverse();
currframe=0;
}

animator::~animator(void)
{
}
