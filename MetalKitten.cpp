#include <stdlib.h>
#include "GL/glut.h"

#include "BallJointBone.h"
#include "HingeJointBone.h"
#include "IKSolver.h"
#include "Chain.h"
#include "HingeChain.h"
#include "GUIUtils.h"
#include "MatrixOps.h"
#include "MassSpringMesh.h"
#include "MassSpringIntegrator.h"
#include "ProjectiveDynamicsSolver.h"

#include "Collision.h"

static int shoulder = 0, elbow = 0;
GUIUtils gui;
bool start=false;
float t = 0;

vector3d arr[7] = { vector3d(0, 0, 0), vector3d(0, 3, 0), vector3d(0, 6, 0), vector3d(0, 9, 0), vector3d(0, 12, 0), vector3d(0, 15, 0), vector3d(0, 18, 0) };
vector3d axes[6] = { vector3d(1, 0, 0), vector3d(0, 1, 0), vector3d(0, 0, 1), vector3d(1, 0, 0), vector3d(0, 1, 0), vector3d(0, 0, 1) };
//vector3d axes2[6] = { vector3d(1, 0, 0), vector3d(1, 0, 0), vector3d(1, 0, 0), vector3d(1, 0, 0), vector3d(1, 0, 0), vector3d(1, 0, 0) };
HingeChain chain(arr,axes, 7);
//Chain chain(arr, 7);
IKSolver solver(chain);

MassSpringMesh mesh;
MassSpringIntegrator inte(&mesh);

TetMesh tetmesh;
ProjectiveDynamicsSolver* psolver;

#define DIM 6


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
		   {
			  //for (int i = 0; i < mesh.getNoMassPoints(); i++)
				//   inte.addExtForce(i, vector3d(0, 9.81 * mesh.getMassPoint(i).mass, 0));
			  // inte.timeStep();
			 
			   psolver->timestep();
			   //start = !start;
		   }

		   tetmesh.renderMesh();
			   //solver.solveByJacobianInverse(gui.getCursorPos());
			   //solver.solveByCCD(gui.getCursorPos());

			   //chain.render();
			  // mesh.render();
			  // inte.collider.render();

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

	  case 'w':
		  gui.getCameraPos().z -= 1;
		  break;
	  case 's':
		  gui.getCameraPos().z += 1;
		  break;
	  case 'd':
		  gui.getCameraPos().x += 1;
		  break;
	  case 'a':
		  gui.getCameraPos().x -= 1;
		  break;
	  case 'e':
		  gui.getCameraPos().y -= 1;
		  break;
	  case 'r':
		  gui.getCameraPos().y += 1;
		  break;
	  case ' ':
		  if (start)
		  {
			  int end = (DIM + 1) * (DIM + 1);
			  int start = end - (DIM + 1) - 1;
			  int end2 = end * 2;
			  int start2 = end2 - (DIM + 1) - 1;

			  for (int i = start; i<end; i++)
				  psolver->setExtForce(i, vector3d(0, 50, 0));

			  for (int i = start2; i<end2; i++)
				  psolver->setExtForce(i, vector3d(0, 50, 0));

			 
		  }
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


void initCloth2()
{
	int n = 10;
	int step = 2;
	float stiff = 1;
	float mass = 0.015;
	for (int i = 0; i < n; i++)
	{
		for (int j = n - 1; j >= 0; j--)
		{
			mesh.addMassPoint(MassPoint(vector3d(i * step - step * n * 0.5, 15, j * step - n * step), mass));
		}
	}




	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n-1; j++)
			mesh.addSpring(Spring(i * n + j, i * n + j + 1, step, stiff));
	}

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n; j++)
			mesh.addSpring(Spring(i * n + j, (i + 1) * n + j, step, stiff));
	}

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			mesh.addSpring(Spring(i * n + j, (i+1) * n + j + 1, sqrt(2.0f * step * step), stiff));
			mesh.addSpring(Spring(i * n + j + 1, (i + 1) * n + j, sqrt(2.0f * step * step), stiff));
		}
	}


	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - 2; j++)
			mesh.addSpring(Spring(i * n + j, i * n + j + 2, step * 2, stiff));
	}

	for (int i = 0; i < n - 2; i++)
	{
		for (int j = 0; j < n; j++)
			mesh.addSpring(Spring(i * n + j, (i + 2) * n + j, step * 2, stiff));
	}

	//tissue paper
	inte.addConstrainedDOF(n * n / 2);
	
	//blanket
	//inte.addConstrainedDOF(0);
	//inte.addConstrainedDOF(n * n - 1);

	//towel
	//inte.addConstrainedDOF(0);
	//inte.addConstrainedDOF(n * n - n);
	
	//inte.addConstrainedDOF(n-1);
	
	
	//inte.addConstrainedDOF(n * n -1);
	//inte.addConstrainedDOF(n * n - n);


	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			int a = i * n + j,
				b = (i + 1) * n + j,
				c = (i + 1) * n + j + 1,
				d = i * n + j + 1;

			inte.collider.addTriangle(a, b, d);
			inte.collider.addTriangle(d, b, c);
		}
	}


	inte.initSolver();

}


void initTetMesh()
{
	int n = DIM + 1;
	int m = DIM + 1;

	for (int k = 0; k<2; k++)
	for (int i = 0; i<n; i++)
	for (int j = 0; j<m; j++)
	{
		tetmesh.addNode( Node(vector3d(i, k, j), 1) );
	

	}

	for (int i = 0; i<n - 1; i++)
	for (int j = 0; j<m - 1; j++)
	{
		//int nextbase = i * m + j;
		//int nexttop = m * n + i * m + j;
		int tet1[4] = { i * m + j, (i + 1) * m + j, i * m + (j + 1), m * n + i * m + j };
		int tet2[4] = { (i + 1) * m + (j + 1), (i + 1) * m + j, i * m + (j + 1), m * n + (i + 1) * m + (j + 1) };
		int tet3[4] = { m * n + i * m + j, m * n + (i + 1) * m + j, m * n + (i + 1) * m + (j + 1), (i + 1) * m + j };
		int tet4[4] = { m * n + i * m + j, m * n + i * m + (j + 1), m * n + (i + 1) * m + (j + 1), i * m + (j + 1) };
		int tet5[4] = { m * n + i * m + j, m * n + (i + 1) * m + (j + 1), i * m + (j + 1), (i + 1) * m + j };

		tetmesh.addTet(tet1, 10000);
		tetmesh.addTet(tet2, 10000);
		tetmesh.addTet(tet3, 10000);
		tetmesh.addTet(tet4, 10000);
		tetmesh.addTet(tet5, 10000);
		
	}

	psolver = new ProjectiveDynamicsSolver(&tetmesh);
	
	for (int i = 0; i<DIM + 1; i++)
		psolver->setContrainedNode(i,true);

	for (int i = (DIM + 1) * (DIM + 1); i<(DIM + 1) * (DIM + 1) + DIM + 1; i++)
		psolver->setContrainedNode(i, true);

	//psolver->setPosition(9, tetmesh.getRestPosition(9) + vector3d(0, 1, 0));
	psolver->init();
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
   


   //float** a = (float**)calloc(3, sizeof(float*));
   //a[0] = (float*)calloc(3, sizeof(float));
   //a[1] = (float*)calloc(3, sizeof(float));
   //a[2] = (float*)calloc(3, sizeof(float));

   //a[0][0] = 1; a[0][1] = 0.2; a[0][2] = 0.3;
   //a[1][0] = 0.2; a[1][1] = 1; a[1][2] = 0.2;
   //a[2][0] = 0.3; a[2][1] = 0.2; a[2][2] = 1;

   //float** o = (float**)calloc(3, sizeof(float*));
   //o[0] = (float*)calloc(3, sizeof(float));
   //o[1] = (float*)calloc(3, sizeof(float));
   //o[2] = (float*)calloc(3, sizeof(float));

   //MatrixOps::InverseMatrix(a, o, 3);

   //for (int i = 0; i < 3; i++)
   //{
	  // for (int j = 0; j < 3; j++)
		 //  printf("%f ", o[i][j]);
	  // printf("\n");
   //}

   //initCloth();

   initTetMesh();


  // vector3d v;
   //if (Collision::testLineTriangle(Line(vector3d(0, 1, 0), vector3d(0, -1, 0)), Triangle(vector3d(-1, 0, 1), vector3d(1, 0, 1), vector3d(0, 0, -1)),&v))
	//  printf("%f %f %f",v.x,v.y,v.z);

   //if (Collision::testLineTriangle(Line(vector3d(0, 1, 0), vector3d(0, -1, 0)), Triangle(vector3d(-1, 0, 1), vector3d(0, 0, -1), vector3d(1, 0, 1))))
	  // printf("Helloo");

   //initCloth2();



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
