/*              The following linux/GLUT port by Jeff Pound, 2004
 *
 *		This code has been created by Banu Octavian aka Choko - 20 may 2000
 *		and uses NeHe tutorials as a starting point (window initialization,
 *		texture loading, GL initialization and code for keypresses) - very good
 *		tutorials, Jeff. If anyone is interested about the presented algorithm
 *		please e-mail me at boct@romwest.ro
 *		Attention!!! This code is not for beginners.
 */


#include <iostream>
#include <GL/glut.h> // GLUT header file
#include <stdio.h>

#include "3Dobject.h"
#include "../LESSON11/M3D_math3d.h"

typedef float GLvector4f[4];							// Typedef's For VMatMult Procedure
typedef float GLmatrix16f[16];							// Typedef's For VMatMult Procedure


bool active= true;		
bool fullscreen= false; 

glObject	obj;										// Object
GLfloat		xrot=0, xspeed=0;							// X Rotation & X Speed
GLfloat		yrot=0, yspeed=0;							// Y Rotation & Y Speed

float LightPos[] = { -5.0f, 4.0f, -3.5f, 1.0f};			// Light Position
float LightAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f};			// Ambient Light Values
float LightDif[] = { 0.6f, 0.6f, 0.6f, 1.0f};			// Diffuse Light Values
float LightSpc[] = {-0.2f, -0.2f, -0.2f, 1.0f};			// Specular Light Values

float MatAmb[] = {0.4f, 0.4f, 0.4f, 1.0f};				// Material - Ambient Values
float MatDif[] = {0.2f, 0.6f, 0.9f, 1.0f};				// Material - Diffuse Values
float MatSpc[] = {0.0f, 0.0f, 0.0f, 1.0f};				// Material - Specular Values
float MatShn[] = {0.0f};								// Material - Shininess

float ObjPos[] = {-2.0f,-2.0f,-5.0f};					// Object Position

GLUquadricObj	*q;										// Quadratic For Drawing A Sphere
float SpherePos[] = {-4.0f,-5.0f,-6.0f};

void VMatMult(GLmatrix16f M, GLvector4f v)
{
  GLfloat res[4];										// Hold Calculated Results
  res[0]=M[ 0]*v[0]+M[ 4]*v[1]+M[ 8]*v[2]+M[12]*v[3];
  res[1]=M[ 1]*v[0]+M[ 5]*v[1]+M[ 9]*v[2]+M[13]*v[3];
  res[2]=M[ 2]*v[0]+M[ 6]*v[1]+M[10]*v[2]+M[14]*v[3];
  res[3]=M[ 3]*v[0]+M[ 7]*v[1]+M[11]*v[2]+M[15]*v[3];
  v[0]=res[0];										// Results Are Stored Back In v[]
  v[1]=res[1];
  v[2]=res[2];
  v[3]=res[3];										// Homogenous Coordinate
}

GLvoid ResizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
  if (height==0)										// Prevent A Divide By Zero By
    {
      height=1;										// Making Height Equal One
    }

  glViewport(0,0,width,height);						// Reset The Current Viewport

  glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
  glLoadIdentity();									// Reset The Projection Matrix

  // Calculate The Aspect Ratio Of The Window
  // 函数gluPerspective的参数near会影响阴影,>0.1f时,阴影会消失;太小时,阴影质量不是很好.
  // http://www.cnblogs.com/gin_dl/archive/2009/08/25/1553636.html
  // http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/the-theory-of-stencil-shadow-volumes-r1873
  gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1,100.0f);

  glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
  glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGLObjects()										// Initialize Objects
{
  if (!ReadObject("Data/Object2.txt", &obj))			// Read Object2 Into obj
    {
      return 0;									// If Failed Return False
    }

  SetConnectivity(&obj);								// Set Face To Face Connectivity

  for (unsigned int i=0;i<obj.nPlanes;i++)			// Loop Through All Object Planes
    CalcPlane(obj, &(obj.planes[i]));				// Compute Plane Equations For All Faces

  return 1;										// Return True
}

int InitGL()										// All Setup For OpenGL Goes Here
{
  if (!InitGLObjects()) return 0;					// Function For Initializing Our Object(s)
  glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
  glClearDepth(1.0f);									// Depth Buffer Setup
  glClearStencil(0);									// Stencil Buffer Setup
  glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
  glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

  glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// Set Light1 Position
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);			// Set Light1 Ambience
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif);			// Set Light1 Diffuse
  glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc);		// Set Light1 Specular
  glEnable(GL_LIGHT1);								// Enable Light1
  glEnable(GL_LIGHTING);								// Enable Lighting

  glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb);			// Set Material Ambience
  glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDif);			// Set Material Diffuse
  glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpc);		// Set Material Specular
  glMaterialfv(GL_FRONT, GL_SHININESS, MatShn);		// Set Material Shininess

  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);								// Set Culling Face To Back Face
  glEnable(GL_CULL_FACE);								// Enable Culling
  glClearColor(0.1f, 1.0f, 0.5f, 1.0f);				// Set Clear Color (Greenish Color)

  q = gluNewQuadric();								// Initialize Quadratic
  gluQuadricNormals(q, GL_SMOOTH);					// Enable Smooth Normal Generation
  gluQuadricTexture(q, GL_FALSE);						// Disable Auto Texture Coords

  return 1;										// Initialization Went OK
}

void DrawGLRoom()										// Draw The Room (Box)
{
  glBegin(GL_QUADS);									// Begin Drawing Quads
  // Floor
  glNormal3f(0.0f, 1.0f, 0.0f);					// Normal Pointing Up
  glVertex3f(-10.0f,-10.0f,-20.0f);				// Back Left
  glVertex3f(-10.0f,-10.0f, 20.0f);				// Front Left
  glVertex3f( 10.0f,-10.0f, 20.0f);				// Front Right
  glVertex3f( 10.0f,-10.0f,-20.0f);				// Back Right
  // Ceiling
  glNormal3f(0.0f,-1.0f, 0.0f);					// Normal Point Down
  glVertex3f(-10.0f, 10.0f, 20.0f);				// Front Left
  glVertex3f(-10.0f, 10.0f,-20.0f);				// Back Left
  glVertex3f( 10.0f, 10.0f,-20.0f);				// Back Right
  glVertex3f( 10.0f, 10.0f, 20.0f);				// Front Right
  // Front Wall
  glNormal3f(0.0f, 0.0f, 1.0f);					// Normal Pointing Away From Viewer
  glVertex3f(-10.0f, 10.0f,-20.0f);				// Top Left
  glVertex3f(-10.0f,-10.0f,-20.0f);				// Bottom Left
  glVertex3f( 10.0f,-10.0f,-20.0f);				// Bottom Right
  glVertex3f( 10.0f, 10.0f,-20.0f);				// Top Right
  // Back Wall
  glNormal3f(0.0f, 0.0f,-1.0f);					// Normal Pointing Towards Viewer
  glVertex3f( 10.0f, 10.0f, 20.0f);				// Top Right
  glVertex3f( 10.0f,-10.0f, 20.0f);				// Bottom Right
  glVertex3f(-10.0f,-10.0f, 20.0f);				// Bottom Left
  glVertex3f(-10.0f, 10.0f, 20.0f);				// Top Left
  // Left Wall
  glNormal3f(1.0f, 0.0f, 0.0f);					// Normal Pointing Right
  glVertex3f(-10.0f, 10.0f, 20.0f);				// Top Front
  glVertex3f(-10.0f,-10.0f, 20.0f);				// Bottom Front
  glVertex3f(-10.0f,-10.0f,-20.0f);				// Bottom Back
  glVertex3f(-10.0f, 10.0f,-20.0f);				// Top Back
  // Right Wall
  glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing Left
  glVertex3f( 10.0f, 10.0f,-20.0f);				// Top Back
  glVertex3f( 10.0f,-10.0f,-20.0f);				// Bottom Back
  glVertex3f( 10.0f,-10.0f, 20.0f);				// Bottom Front
  glVertex3f( 10.0f, 10.0f, 20.0f);				// Top Front
  glEnd();											// Done Drawing Quads
}

void DrawGLScene()									// Main Drawing Routine
{
  GLmatrix16f Minv, Inv;
  GLvector4f  lp;

  // Clear Color Buffer, Depth Buffer, Stencil Buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glColor4f(0.7f, 0.4f, 0.0f, 1.0f);
  glLoadIdentity();

  glPushMatrix(); {
      glTranslatef(0.0f, 0.0f, -20.0f);					// Zoom Into Screen 20 Units
      glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// Position Light1
      glTranslatef(SpherePos[0], SpherePos[1], SpherePos[2]);	// Position The Sphere
      gluSphere(q, 1.5f, 32, 16);							// Draw A Sphere
  } glPopMatrix();

  glPushMatrix(); {
      glTranslatef(0.0f, 0.0f, -20.0f);					// Zoom Into The Screen 20 Units
      DrawGLRoom();										// Draw The Room
      glTranslatef(ObjPos[0], ObjPos[1], ObjPos[2]);		// Position The Object
      glRotatef(xrot, 1.0f, 0.0f, 0.0f);					// Spin It On The X Axis By xrot
      glRotatef(yrot, 0.0f, 1.0f, 0.0f);					// Spin It On The Y Axis By yrot
      DrawGLObject(obj);									// Procedure For Drawing The Loaded Object

      // 获得光源的位置(返回值是在视觉坐标系下的)
      glGetLightfv(GL_LIGHT1, GL_POSITION, lp);
      glGetFloatv(GL_MODELVIEW_MATRIX, Minv);
      m3dInvertMatrix44(Inv, Minv);
      // 把光源位置从视觉坐标系下变换到当前的场景坐标系(obj)下.
      VMatMult(Inv, lp);
      // 开始渲染阴影
      CastShadow(&obj, lp);

      // 用一个小黄球标记出光源的位置
      glPushMatrix(); {
          glColor4f(0.7f, 0.4f, 0.0f, 1.0f);					// Set Color To Purplish Blue
          glDisable(GL_LIGHTING);								// Disable Lighting
          glDepthMask(GL_FALSE);								// Disable Depth Mask
          glTranslatef(lp[0], lp[1], lp[2]);
          gluSphere(q, 0.2f, 16, 8);
          glEnable(GL_LIGHTING);								// Enable Lighting
          glDepthMask(GL_TRUE);								// Enable Depth Mask
      } glPopMatrix();
  } glPopMatrix();

  xrot += xspeed;	
  yrot += yspeed;	

  glFlush();
  glutSwapBuffers();
}

void SpecialFunc(int key, int x, int y)					// Process Key Presses
{
  switch(key) {
  case GLUT_KEY_LEFT:
    yspeed -= 0.1f;					// 'Arrow Left' Decrease yspeed
    break;
  case GLUT_KEY_RIGHT:
    yspeed += 0.1f;					// 'Arrow Right' Increase yspeed
    break;
  case GLUT_KEY_UP:
    xspeed -= 0.1f;					// 'Arrow Up' Decrease xspeed
    break;
  case GLUT_KEY_DOWN:
    xspeed += 0.1f;					// 'Arrow Down' Increase xspeed
    break;
  default:
    break;

  }
}

void KeyFunc(unsigned char key, int x, int y)
{
  switch(key) {
    // Adjust Light's Position
  case 'L': 
  case 'l': 
    LightPos[0] += 0.05f;				// 'L' Moves Light Right
    break;
  case 'J': 
  case 'j': 
    LightPos[0] -= 0.05f;				// 'J' Moves Light Left
    break;

  case 'I': 
  case 'i': 
    LightPos[1] += 0.05f;				// 'I' Moves Light Up
    break;
  case 'K': 
  case 'k': 
    LightPos[1] -= 0.05f;				// 'K' Moves Light Down
    break;

  case 'O': 
  case 'o': 
    LightPos[2] += 0.05f;				// 'O' Moves Light Toward Viewer
    break;
  case 'U': 
  case 'u': 
    LightPos[2] -= 0.05f;				// 'U' Moves Light Away From Viewer
    break;

    // Adjust Object's Position
  case '6': 
    ObjPos[0] += 0.05f;			// 'Numpad6' Move Object Right
    break;
  case '4': 
    ObjPos[0] -= 0.05f;			// 'Numpad4' Move Object Left
    break;

  case '8':
    ObjPos[1] += 0.05f;			// 'Numpad8' Move Object Up
    break;
  case '5':
    ObjPos[1] -= 0.05f;			// 'Numpad5' Move Object Down
    break;

  case '9':
    ObjPos[2] += 0.05f;			// 'Numpad9' Move Object Toward Viewer
    break;
  case '7':
    ObjPos[2] -= 0.05f;			// 'Numpad7' Move Object Away From Viewer
    break;

    // Adjust Ball's Position
  case 'D':
  case 'd':
    SpherePos[0] += 0.05f;				// 'D' Move Ball Right
    break;
  case 'A':
  case 'a':
    SpherePos[0] -= 0.05f;				// 'A' Move Ball Left
    break;

  case 'W':
  case 'w':
    SpherePos[1] += 0.05f;				// 'W' Move Ball Up
    break;
  case 'S':
  case 's':
    SpherePos[1] -= 0.05f;				// 'S' Move Ball Down
    break;

  case 'E':
  case 'e':
    SpherePos[2] += 0.05f;				// 'E' Move Ball Toward Viewer
    break;
  case 'Q':
  case 'q':
    SpherePos[2] -= 0.05f;				// 'Q' Move Ball Away From Viewer
    break;
  case 27:    //Escape key
    exit(0);
  default:
    break;
  }
}

// initialize everything and enter the main loop
int main(int argc, char** argv) {

  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_STENCIL | GLUT_DEPTH );
  glutCreateWindow("Banu Octavian & NeHe's Shadow Casting Tutorial");
  glutDisplayFunc(DrawGLScene);
  glutIdleFunc(DrawGLScene);
  glutKeyboardFunc(KeyFunc);
  glutSpecialFunc(SpecialFunc);
  glutReshapeFunc(ResizeGLScene);
  
  if(!InitGL()) {
    printf("Failed to initialize GL\n");
    return 1;
  }

  glutMainLoop();

  return 0;
}
