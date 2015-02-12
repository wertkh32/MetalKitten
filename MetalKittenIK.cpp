#include <stdlib.h>
#include "GL/glut.h"

#include "BallJointBone.h"
#include "HingeJointBone.h"
#include "IKSolver.h"
#include "Chain.h"
#include "HingeChain.h"
#include "GUIUtils.h"

static int shoulder = 0, elbow = 0;
GUIUtils gui;
bool start=false;

vector3d arr[5] = { vector3d(0, 0, 0), vector3d(0, 3, 0), vector3d(0, 7, 0), vector3d(0, 9, 0), vector3d(0, 12, 0) };
vector3d axes[4] = { vector3d(1, 0, 0), vector3d(1, 0, 0), vector3d(1, 0, 0), vector3d(1, 0, 0) };
//HingeChain chain(arr,axes, 5);
Chain chain(arr, 5);
IKSolver solver(chain);

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
   glClearColor (1.0, 1.0, 1.0, 1.0);
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
   
		   vector3d campos = -gui.getCameraPos();
		   glTranslatef(campos.x,campos.y,campos.z);
		   glMultMatrixf((gui.getSceneRotation().toRotMatrix()).arr);

		   glColor3f(1,0,0);

		   if (start)
			   solver.solveByJacobianInverse(gui.getCursorPos());
			   //solver.solveByCCD(gui.getCursorPos());

		   chain.render();

		   drawfloor();
		   gui.renderCursor();
   glPopMatrix();

 

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
   gui.onKeyPress(key, x, y);
   
   switch (key) {
      case 'm':
         start=!start;
		
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
	gui.onMouseMove(x, y);
}

int main(int argc, char** argv)
{

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
