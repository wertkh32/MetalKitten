#include <stdlib.h>
#include <GL/glut.h>
#include "model.h"
#include "animator.h"

static int shoulder = 0, elbow = 0;
model* m;
animator *ani_larm, *ani_lfarm, 
	     *ani_rarm, *ani_rfarm,
		 *ani_head,
		 *ani_lleg, *ani_lcalf,
		 *ani_rleg, *ani_rcalf;
GUIUtils gui;
bool start=false;

void lettherebelight(){

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

const GLfloat high_emission[] = { 0.3f,0.3f,0.7f,1.0f };
 // glClearColor(1,1,1,1);
  

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
   //glMaterialfv(GL_FRONT, GL_EMISSION, high_emission);

}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);   

   glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	 glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
   lettherebelight();
}

void drawfloor(){
glColor3f(0.5,0.5,0.5);
glBegin(GL_QUADS);
glNormal3f(0,1,0);
glVertex3f(-100,FLOOR,-100);
glVertex3f(-100,FLOOR, 100);
glVertex3f(100,FLOOR, 100);
glVertex3f(100,FLOOR,-100);
glEnd();
}

void display(void)
{
	static float k=0;
	k+=1;
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
  
   glPushMatrix();
   
   

   glTranslatef(0,0,-20);
   glRotatef(gui.getSceneRotation().x,0,1,0);


   if(start){
   if(!ani_head->animate())ani_head->reverse();

   if(!ani_lfarm->animate())ani_lfarm->reverse();
   if(!ani_rfarm->animate())ani_rfarm->reverse();

   if(!ani_larm->animate())ani_larm->reverse();
   if(!ani_rarm->animate())ani_rarm->reverse();

    if(!ani_lcalf->animate())ani_lcalf->reverse();
   if(!ani_rcalf->animate())ani_rcalf->reverse();

   if(!ani_lleg->animate())ani_lleg->reverse();
   if(!ani_rleg->animate())ani_rleg->reverse();
   }

   glColor3f(1,0,0);
   m->draw();
   drawfloor();
   
   glPopMatrix();

   gui.renderCursor();

   glFlush ();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(75.0, (GLfloat) w/(GLfloat) h, 1.0, 50.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
  // glTranslatef (0.0, 0.0, -8.0);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'm':
         start=true;
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}



void timer(int fps) {
    glutPostRedisplay();
    glutTimerFunc(1000/fps, timer, fps);
}

void mouseFunc(int button, int state, int x, int y)
{
	gui.onMouseClick(button,state, x, y);
}

void mouseMoveFunc(int x, int y)
{
	//printf("%d %d\n",x, y);
	//vector3d v = GUIUtils::get3DFrom2D(x, y);
	//printf("%f %f %f", v.x, v.y, v.z);
	gui.onMouseMove(x, y);
}

int main(int argc, char** argv)
{
	m=new model("DUDE.raw");

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();

   printf("Press M to start");

   glutTimerFunc(1000/60, timer, 60);
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouseFunc);
   glutMotionFunc(mouseMoveFunc);
   glutPassiveMotionFunc(mouseMoveFunc);
   glutMainLoop();
   return 0;
}
