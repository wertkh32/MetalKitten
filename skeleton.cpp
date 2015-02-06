#include "skeleton.h"


skeleton::skeleton()
{
	child=0;
	child_no=0;
	vertices.alloc(5000);
	bb=0;
}

void skeleton::setAABB(boundsAABB* bbb){
	bb=bbb;
}

void skeleton::addVertex(vector3d& v,float weight){
	if(bb->ptInBounds(v))
		vertices.push(make_pair(&v,weight));
}

void skeleton::makeChild(int child_num){
	if(!child){
	child= new skeleton[child_num];
	child_no=child_num;
	}
}

void skeleton::rotateAllVertices(vector3d& centre,Matrix4d& rotation){
	for(int i=0;i<child_no;i++)
		child[i].rotateAllVertices(centre,rotation);

	rotate(startpt, centre, rotation,1);
	
	for(int i=0;i<vertices.size();i++)
		if(LINEAR_BLEND)rotate(*vertices[i].first, centre, rotation,vertices[i].second);
		else rotate(*vertices[i].first, centre, rotation);
}

void skeleton::rotateAllVertices(){
	Matrix4d mrot = rot.toRotMatrix();
	for(int i=0;i<child_no;i++)
		child[i].rotateAllVertices(startpt,mrot);


	for(int i=0;i<vertices.size();i++)
		if(LINEAR_BLEND)rotate(*vertices[i].first,startpt,mrot,vertices[i].second);
		else rotate(*vertices[i].first, startpt,mrot);

}


skeleton::~skeleton(void)
{
}
