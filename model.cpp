#include "model.h"


model::model(char* filename)
{
	float x,y,z; 
	FILE*f=fopen(filename,"r");
    vertices.alloc(30000);
	normals.alloc(30000);
    while(fscanf(f,"%f %f %f",&x, &y, &z)!=EOF)
		vertices.push(vector3d(x,y,z));
 
	for(int i=0;i<vertices.size();i+=4){
	normals[i]=normals[i+1]=normals[i+2]=normals[i+3]=(vertices[i+2]-vertices[i]).cross(vertices[i+3]-vertices[i]);
	}

    fclose(f);

	hip=&skel;

	hip->makeChild(3);
	torso=&(hip->child[0]);
	lleg=&(hip->child[1]);
	rleg=&(hip->child[2]);

	torso->makeChild(3);
	head=&(torso->child[0]);
	larm=&(torso->child[1]);
	rarm=&(torso->child[2]);

	larm->makeChild(1);
	lfarm=&(larm->child[0]);

	rarm->makeChild(1);
	rfarm=&(rarm->child[0]);

	lleg->makeChild(1);
	lcalf=&(lleg->child[0]);

	rleg->makeChild(1);
	rcalf=&(rleg->child[0]);

	boundsAABB laJoint(-2.6,-1.5,-2,-2.4,1,2);
	larm->setAABB(new boundsAABB(-5.2,-1.5,-2,-2.4,1,2));
	boundsAABB lfaJoint(-5.2,-1,-2,-5,1,2);
	lfarm->setAABB(new boundsAABB(-10,-1,-2,-5,1,2));
	
	boundsAABB raJoint(2.4,-1.5,-2,2.6,1,2);
	rarm->setAABB(new boundsAABB(2.4,-1.5,-2,5.2,1,2));
	boundsAABB rfaJoint(5,-1,-2,5.2,1,2);
	rfarm->setAABB(new boundsAABB(5,-1,-2,10,1,2));

	lleg->setAABB(new boundsAABB(-3,-9,-2,-0.1,-5,2));
	lcalf->setAABB(new boundsAABB(-3,-13,-2,-0.1,-9,2));

	rleg->setAABB(new boundsAABB(0.1,-9,-2,3,-5,2));
	rcalf->setAABB(new boundsAABB(0.1,-13,-2,3,-9,2));

	head->setAABB(new boundsAABB(-3,1,-3,3,6,3));
	hip->setAABB(new boundsAABB(-3,-8,-2,3,-5,2));
	torso->setAABB(new boundsAABB(-2.6,-5,-2,2.6,1,2));

	for(int i=0;i<vertices.size();i++){
		if(lfaJoint.ptInBounds(vertices[i])){
		larm->addVertex(vertices[i],0.5);
		lfarm->addVertex(vertices[i],0.5);
		}
		else if(rfaJoint.ptInBounds(vertices[i])){
		rarm->addVertex(vertices[i],0.5);
		rfarm->addVertex(vertices[i],0.5);
		}
		else if(laJoint.ptInBounds(vertices[i])){
		larm->addVertex(vertices[i],0.5);
		torso->addVertex(vertices[i],0.5);
		}
		else if(raJoint.ptInBounds(vertices[i])){
		rarm->addVertex(vertices[i],0.5);
		torso->addVertex(vertices[i],0.5);
		}
		else{
		larm->addVertex(vertices[i],1);
		rarm->addVertex(vertices[i],1);
		lfarm->addVertex(vertices[i],1);
		rfarm->addVertex(vertices[i],1);
		lleg->addVertex(vertices[i],1);
		lcalf->addVertex(vertices[i],1);
		rleg->addVertex(vertices[i],1);
		rcalf->addVertex(vertices[i],1);
		hip->addVertex(vertices[i],1);
		head->addVertex(vertices[i],1);
		torso->addVertex(vertices[i],1);
		}
	}

	hip->tag="HIP";
	hip->startpt=vector3d(0,-3,1);
	//hip->endpt=vector3d(0,-5,0);

	torso->tag="TORSO";
	torso->startpt=vector3d(0,-3,0);
	//torso->endpt=vector3d(0,1,0);

	head->tag="HEAD";
	head->startpt=vector3d(0,1,0);
	//head->endpt=vector3d(0,3,0);

	larm->tag="LEFT ARM";
	larm->startpt=vector3d(-2.3,0.5,0);
	//larm->endpt=vector3d(0,1,0);

	rarm->tag="RIGHT ARM";
	rarm->startpt=vector3d(2.3,0.5,0);
	//rarm->endpt=vector3d(0,1,0);

	lfarm->tag="LEFT FORE ARM";
	lfarm->startpt=vector3d(-5,0.5,0);
	//lfarm->endpt=vector3d(0,1,0);

	rfarm->tag="RIGHT FORE ARM";
	rfarm->startpt=vector3d(5,0.5,0);
	//rfarm->endpt=vector3d(0,1,0);

	lleg->tag="LEFT LEG";
	lleg->startpt=vector3d(-0.3,-5,0);
	//lleg->endpt=vector3d(0,1,0);

	rleg->tag="RIGHT LEG";
	rleg->startpt=vector3d(0.3,-5,0);
	//rleg->endpt=vector3d(0,1,0);

	lcalf->tag="LEFT CALF";
	lcalf->startpt=vector3d(-0.3,-9,0);
	//lcalf->endpt=vector3d(0,1,0);

	rcalf->tag="RIGHT CALF";
	rcalf->startpt=vector3d(0.3,-9,0);
	//rcalf->endpt=vector3d(0,1,0);

}

void model::addrotation(skeleton* part, float x_angle, float y_angle, float z_angle){
part->rot= quatn(x_angle,y_angle,z_angle);
part->rotateAllVertices();
}

void model::addrotation(skeleton* part, quatn& q){
	part->rot = q;
	part->rotateAllVertices();
}

void model::draw(){

glPushMatrix();

//Turn on for more correct graphics. Then again, this is a animation demo
for(int i=0;i<vertices.size();i+=4){
	normals[i]=normals[i+1]=normals[i+2]=normals[i+3]=(vertices[i+2]-vertices[i]).cross(vertices[i+3]-vertices[i]);
}

glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glNormalPointer(GL_FLOAT,0,&normals[0]);
glDrawArrays(GL_QUADS,0,vertices.size());


larm->bb->drawBounds();
rarm->bb->drawBounds();
lfarm->bb->drawBounds();
rfarm->bb->drawBounds();
lleg->bb->drawBounds();
lcalf->bb->drawBounds();
rleg->bb->drawBounds();
rcalf->bb->drawBounds();
hip->bb->drawBounds();
head->bb->drawBounds();
torso->bb->drawBounds();

glPopMatrix();
}
/*
void model::reloadmodel(){
for(int i=0;i<original.size();i++)
	vertices[i]=original[i];
}*/

model::~model(void)
{
}
