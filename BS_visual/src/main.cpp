#include "MainWindow.h"
#include <QApplication>

#include "ScatteringScene.h"
#include "MyGLWidget.h"
#include "ArgPP.h"

/**
 * TODO delete .qrc files
 * TODO delete ParticleModel/View
 */

//int main(int argc, char *argv[])
//{
//	QApplication a(argc, argv);
////	MainWindow w;
////	w.show();

//	QSurfaceFormat format;
//	format.setDepthBufferSize(24);
//	format.setStencilBufferSize(8);
//	format.setVersion(3, 2);
//	format.setProfile(QSurfaceFormat::CoreProfile);
//	QSurfaceFormat::setDefaultFormat(format);

//	MyGLWidget widget;
//	widget.show();

//	return a.exec();
//}
/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#include <windows.h>  // for MS Windows
#include <GL/glut.h>

/* Global variables */
char title[] = "3D Shapes";

/* Initialize OpenGL Graphics */
void initGL() {
//   glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Set background color to black And opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Called back when the window first appears And
   whenever the window needs to be re-painted. */
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color And depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
   glTranslatef(1.5f, 0.0f, -7.0f);  // Move right And into the screen

   glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	  // Top face (y = 1.0f)
	  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	  glColor3f(0.0f, 1.0f, 0.0f);     // Green
	  glVertex3f( 1.0f, 1.0f, -1.0f);
	  glVertex3f(-1.0f, 1.0f, -1.0f);
	  glVertex3f(-1.0f, 1.0f,  1.0f);
	  glVertex3f( 1.0f, 1.0f,  1.0f);

	  // Bottom face (y = -1.0f)
	  glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	  glVertex3f( 1.0f, -1.0f,  1.0f);
	  glVertex3f(-1.0f, -1.0f,  1.0f);
	  glVertex3f(-1.0f, -1.0f, -1.0f);
	  glVertex3f( 1.0f, -1.0f, -1.0f);

	  // Front face  (z = 1.0f)
	  glColor3f(1.0f, 0.0f, 0.0f);     // Red
	  glVertex3f( 1.0f,  1.0f, 1.0f);
	  glVertex3f(-1.0f,  1.0f, 1.0f);
	  glVertex3f(-1.0f, -1.0f, 1.0f);
	  glVertex3f( 1.0f, -1.0f, 1.0f);

	  // Back face (z = -1.0f)
	  glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	  glVertex3f( 1.0f, -1.0f, -1.0f);
	  glVertex3f(-1.0f, -1.0f, -1.0f);
	  glVertex3f(-1.0f,  1.0f, -1.0f);
	  glVertex3f( 1.0f,  1.0f, -1.0f);

	  // Left face (x = -1.0f)
	  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	  glVertex3f(-1.0f,  1.0f,  1.0f);
	  glVertex3f(-1.0f,  1.0f, -1.0f);
	  glVertex3f(-1.0f, -1.0f, -1.0f);
	  glVertex3f(-1.0f, -1.0f,  1.0f);

	  // Right face (x = 1.0f)
	  glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	  glVertex3f(1.0f,  1.0f, -1.0f);
	  glVertex3f(1.0f,  1.0f,  1.0f);
	  glVertex3f(1.0f, -1.0f,  1.0f);
	  glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();  // End of drawing color-cube

   // Render a pyramid consists of 4 triangles
   glLoadIdentity();                  // Reset the model-view matrix
   glTranslatef(-1.5f, 0.0f, -6.0f);  // Move left And into the screen

   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
	  // Front
	  glColor3f(1.0f, 0.0f, 0.0f);     // Red
	  glVertex3f( 0.0f, 1.0f, 0.0f);
	  glColor3f(0.0f, 1.0f, 0.0f);     // Green
	  glVertex3f(-1.0f, -1.0f, 1.0f);
	  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	  glVertex3f(1.0f, -1.0f, 1.0f);

	  // Right
	  glColor3f(1.0f, 0.0f, 0.0f);     // Red
	  glVertex3f(0.0f, 1.0f, 0.0f);
	  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	  glVertex3f(1.0f, -1.0f, 1.0f);
	  glColor3f(0.0f, 1.0f, 0.0f);     // Green
	  glVertex3f(1.0f, -1.0f, -1.0f);

	  // Back
	  glColor3f(1.0f, 0.0f, 0.0f);     // Red
	  glVertex3f(0.0f, 1.0f, 0.0f);
	  glColor3f(0.0f, 1.0f, 0.0f);     // Green
	  glVertex3f(1.0f, -1.0f, -1.0f);
	  glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	  glVertex3f(-1.0f, -1.0f, -1.0f);

	  // Left
	  glColor3f(1.0f,0.0f,0.0f);       // Red
	  glVertex3f( 0.0f, 1.0f, 0.0f);
	  glColor3f(0.0f,0.0f,1.0f);       // Blue
	  glVertex3f(-1.0f,-1.0f,-1.0f);
	  glColor3f(0.0f,1.0f,0.0f);       // Green
	  glVertex3f(-1.0f,-1.0f, 1.0f);
   glEnd();   // Done drawing the pyramid

   glutSwapBuffers();  // Swap the front And back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears And
   whenever the window is re-sized with its new width And height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear And zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv)
{
//   glutInit(&argc, argv);            // Initialize GLUT
//   glutCreateWindow(title);          // Create window with the given title
//	ArgPP parcer;
//	parcer.AddRule("f", 1);

	MyGLWidget mw("out15_mbs.dat");
	mw.display();

//   ::glutDisplayFunc(/*display*/MyGLWidget::paintGL());       // Register callback handler for window re-paint event
//   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   return 0;
}
/*
 *  Constructive Solid Geometry program based on information from the
 *  Advanced OpenGL Rendering course at SIGGRAPH '96 by David Blythe And
 *  Tom McReynolds.  Click right mouse button for menu.
 *
 *  author: Nate Robins
 *  email: ndr@pobox.com
 *  www: http://www.pobox.com/~ndr
 */


///* includes */
//#include <GL/glut.h>
//#include <math.h>
//#include "trackball.h"


///* defines */
//#define SPHERE 1
//#define CONE   2
//#define CUBE   3


///* csgOperation
// *  types of CSG operations
// */
//typedef enum {
//	CSG_A,
//	CSG_B,
//	CSG_A_OR_B,
//	CSG_A_AND_B,
//	CSG_A_SUB_B,
//	CSG_B_SUB_A
//} csgOperation;


///* globals */
//GLint Width;
//GLint Height;

//GLfloat zoom = 0.0;

//GLboolean cube_picked   = GL_FALSE;
//GLboolean sphere_picked = GL_FALSE;
//GLboolean cone_picked   = GL_FALSE;

//GLboolean selection     = GL_FALSE;	/* rendering to selection buffer */

//#define SELECT_BUFFER 32
//GLuint select_buffer[SELECT_BUFFER];	/* selection buffer */

//GLfloat cone_x = 0.0;
//GLfloat cone_y = 0.0;
//GLfloat cone_z = 0.0;

//GLfloat cube_x = 0.0;
//GLfloat cube_y = 0.0;
//GLfloat cube_z = 0.0;

//GLfloat sphere_x = 0.0;
//GLfloat sphere_y = 0.0;
//GLfloat sphere_z = 0.0;

//GLint mouse_state = -1;
//GLint mouse_button = -1;

//csgOperation Op = CSG_A_OR_B;

//void (*A)(void);
//void (*B)(void);


///* functions */

///* one()
// *  draw a single object
// */
//void one(void(*a)(void))
//{
//  glEnable(GL_DEPTH_TEST);
//  a();
//  glDisable(GL_DEPTH_TEST);
//}

///* Or()
// *  boolean A Or B (draw wherever A Or B)
// *  algorithm: simple, just draw both with depth test enabled
// */
//void Or(void(*a)(void), void(*b)())
//{
//	glPushAttrib(GL_ALL_ATTRIB_BITS);  /* TODO - should just push depth */
//	glEnable(GL_DEPTH_TEST);
//	a(); b();
//	glPopAttrib();
//}

///* inside()
// *  sets stencil buffer to show the part of A
// *  (front Or back face according to 'face')
// *  that is inside of B.
// */
//void inside(void(*a)(void), void(*b)(void), GLenum face, GLenum test)
//{
//	/* draw A into depth buffer, but not into color buffer */
//	glEnable(GL_DEPTH_TEST);
//	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//	glCullFace(face);
//	a();

//	/* use stencil buffer to find the parts of A that are inside of B
//	 * by first incrementing the stencil buffer wherever B's front faces
//	 * are...
//	 */
//	glDepthMask(GL_FALSE);
//	glEnable(GL_STENCIL_TEST);
//	glStencilFunc(GL_ALWAYS, 0, 0);
//	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
//	glCullFace(GL_BACK);
//	b();

//	/* ...then decrement the stencil buffer wherever B's back faces are */
//	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
//	glCullFace(GL_FRONT);
//	b();

//	/* now draw the part of A that is inside of B */
//	glDepthMask(GL_TRUE);
//	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//	glStencilFunc(test, 0, 1);
//	glDisable(GL_DEPTH_TEST);
//	glCullFace(face);
//	a();

//	/* reset stencil test */
//	glDisable(GL_STENCIL_TEST);
//}

///* fixup()
// *  fixes up the depth buffer with A's depth values
// */
//void fixup(void(*a)(void))
//{
//	/* fix up the depth buffer */
//	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_STENCIL_TEST);
//	glDepthFunc(GL_ALWAYS);
//	a();

//	/* reset depth func */
//	glDepthFunc(GL_LESS);
//}

///* And()
// *  boolean A And B (draw wherever A intersects B)
// *  algorithm: find where A is inside B, then find where
// *             B is inside A
// */
//void And(void(*a)(void), void(*b)(void))
//{
//	inside(a, b, GL_BACK, GL_NOTEQUAL);
//#if 1  /* set to 0 for faster, but incorrect results */
//	fixup(b);
//#endif
//	inside(b, a, GL_BACK, GL_NOTEQUAL);
//}

///*
// * sub()
// *  boolean A subtract B (draw wherever A is And B is NOT)
// *  algorithm: find where a is inside B, then find where
// *             the BACK faces of B are NOT in A
// */
//void sub(void(*a)(void), void(*b)(void))
//{
//	inside(a, b, GL_FRONT, GL_NOTEQUAL);
//#if 1  /* set to 0 for faster, but incorrect results */
//	fixup(b);
//#endif
//	inside(b, a, GL_BACK, GL_EQUAL);
//}

///* sphere()
// *  draw a yellow sphere
// */
//void sphere(void)
//{
//	glLoadName(2);
//	glPushMatrix();
//	glTranslatef(sphere_x, sphere_y, sphere_z);
//	glColor3f(1.0, 1.0, 0.0);
//	glutSolidSphere(5.0, 16, 16);
//	glPopMatrix();
//}

///* cube()
// *  draw a red cube
// */
//void cube(void)
//{
//	glLoadName(1);
//	glPushMatrix();
//	glTranslatef(cube_x, cube_y, cube_z);
//	glColor3f(1.0, 0.0, 0.0);
//	glutSolidCube(8.0);
//	glPopMatrix();
//}

///* cone()
// *  draw a green cone
// */
//void cone(void)
//{
//	glLoadName(3);
//	glPushMatrix();
//	glTranslatef(cone_x, cone_y, cone_z);
//	glColor3f(0.0, 1.0, 0.0);
//	glTranslatef(0.0, 0.0, -6.5);
//	glutSolidCone(4.0, 15.0, 16, 16);
//	glRotatef(180.0, 1.0, 0.0, 0.0);
//	glutSolidCone(4.0, 0.0, 16, 1);
//	glPopMatrix();
//}

//void init(void)
//{
//	GLfloat lightposition[] = { -3.0, 3.0, 3.0, 0.0 };

//	tbInit(GLUT_MIDDLE_BUTTON);

//	glSelectBuffer(SELECT_BUFFER, select_buffer);

//	glDepthFunc(GL_LESS);
//	glEnable(GL_DEPTH_TEST);

//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHTING);
//	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

//	glEnable(GL_COLOR_MATERIAL);

//	glEnable(GL_CULL_FACE);

//	glClearColor(0.0, 0.0, 1.0, 0.0);
//}

//void reshape(int width, int height)
//{
//	Width = width;
//	Height = height;

//	tbReshape(width, height);

//	glViewport(0, 0, width, height);

//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glFrustum(-3.0, 3.0, -3.0, 3.0, 64, 256);

//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glTranslatef(0.0, 0.0, -200.0 + zoom);
//}

//void display(void)
//{
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//  glPushMatrix();
//  tbMatrix();

//  switch(Op) {
//  case CSG_A:
//	one(A);
//	break;
//  case CSG_B:
//	one(B);
//	break;
//  case CSG_A_OR_B:
//	Or(A, B);
//	break;
//  case CSG_A_AND_B:
//	And(A, B);
//	break;
//  case CSG_A_SUB_B:
//	sub(A, B);
//	break;
//  case CSG_B_SUB_A:
//	sub(B, A);
//	break;
//  }

//  glPopMatrix();

//  if (!selection)
//	glutSwapBuffers();
//}

//GLuint
//pick(int x, int y)
//{
//  GLuint    i, hits, num_names, picked;
//  GLuint*   p;
//  GLboolean save;
//  GLuint    depth = (GLuint)-1;
//  GLint     viewport[4];

//  /* get the current viewport parameters */
//  glGetIntegerv(GL_VIEWPORT, viewport);

//  /* set the render mode to selection */
//  glRenderMode(GL_SELECT);
//  selection = GL_TRUE;
//  glInitNames();
//  glPushName(0);

//  /* setup a picking matrix And render into selection buffer */
//  glMatrixMode(GL_PROJECTION);
//  glPushMatrix();

//  glLoadIdentity();
//  gluPickMatrix(x, viewport[3] - y, 5.0, 5.0, viewport);
//  glFrustum(-3.0, 3.0, -3.0, 3.0, 64, 256);

//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//  glTranslatef(0.0, 0.0, -200.0 + zoom);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//  glPushMatrix();
//  tbMatrix();
//  Or(A, B);
//  glPopMatrix();

//  glMatrixMode(GL_PROJECTION);
//  glPopMatrix();
//  glMatrixMode(GL_MODELVIEW);

//  hits = glRenderMode(GL_RENDER);

//  selection = GL_FALSE;

//  p = select_buffer;
//  picked = 0;
//  for (i = 0; i < hits; i++) {
//	save = GL_FALSE;
//	num_names = *p;			/* number of names in this hit */
//	p++;

//	if (*p <= depth) {			/* check the 1st depth value */
//	  depth = *p;
//	  save = GL_TRUE;
//	}
//	p++;
//	if (*p <= depth) {			/* check the 2nd depth value */
//	  depth = *p;
//	  save = GL_TRUE;
//	}
//	p++;

//	if (save)
//	  picked = *p;

//	p += num_names;			/* skip over the rest of the names */
//  }

//  return picked;
//}

//void keyboard(unsigned char key, int x, int y)
//{
//	switch(key) {
//	case 'c':
//	if(A == cube && B == sphere) {
//		A = sphere;
//		B = cone;
//	} else if(A == sphere && B == cone) {
//		A = cone;
//		B = cube;
//	} else { /* if(A == cone && B = cube) */
//		A = cube;
//		B = sphere;
//	}
//	break;
//	case 'a':
//	Op = CSG_A;
//	break;
//	case 'b':
//	Op = CSG_B;
//	break;
//	case '|':
//	Op = CSG_A_OR_B;
//	break;
//	case '&':
//	Op = CSG_A_AND_B;
//	break;
//	case '-':
//	Op = CSG_A_SUB_B;
//	break;
//	case '_':
//	Op = CSG_B_SUB_A;
//	break;
//	case 'z':
//	zoom -= 6.0;
//	reshape(Width, Height);
//	break;
//	case 'Z':
//	zoom += 6.0;
//	reshape(Width, Height);
//	break;
//	case 27:
//	exit(0);
//	break;
//	case '\r':
//	break;
//	default:
//	return;
//	}

//	glutPostRedisplay();
//}

//void
//mouse(int button, int state, int x, int y)
//{
//  mouse_state = state;
//  mouse_button = button;

//  tbMouse(button, state, x, y);

//  if (button == GLUT_LEFT_BUTTON) {
//	switch (pick(x, y)) {
//	case 1:
//	  cube_picked = GL_TRUE;
//	  sphere_picked = GL_FALSE;
//	  cone_picked = GL_FALSE;
//	  break;

//	case 2:
//	  sphere_picked = GL_TRUE;
//	  cube_picked = GL_FALSE;
//	  cone_picked = GL_FALSE;
//	  break;

//	case 3:
//	  cone_picked = GL_TRUE;
//	  sphere_picked = GL_FALSE;
//	  cube_picked = GL_FALSE;
//	  break;

//	default:
//	  sphere_picked = GL_FALSE;
//	  cube_picked = GL_FALSE;
//	  cone_picked = GL_FALSE;
//	  break;
//	}
//  }

//  glutPostRedisplay();
//}

//GLvoid
//cross(GLfloat* u, GLfloat* v, GLfloat* n)
//{
//  /* compute the cross product (u x v for right-handed [ccw]) */
//  n[0] = u[1] * v[2] - u[2] * v[1];
//  n[1] = u[2] * v[0] - u[0] * v[2];
//  n[2] = u[0] * v[1] - u[1] * v[0];
//}

//GLfloat
//normalize(GLfloat* n)
//{
//  GLfloat l;

//  /* normalize */
//  l = (GLfloat)sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
//  n[0] /= l;
//  n[1] /= l;
//  n[2] /= l;

//  return l;
//}

//void
//motion(int x, int y)
//{
//  GLdouble model[4*4];
//  GLdouble proj[4*4];
//  GLint view[4];
//  GLdouble pan_x, pan_y, pan_z;

//  tbMotion(x, y);

//  if (mouse_state == GLUT_DOWN && mouse_button == GLUT_LEFT_BUTTON) {
//	glGetDoublev(GL_MODELVIEW_MATRIX, model);
//	glGetDoublev(GL_PROJECTION_MATRIX, proj);
//	glGetIntegerv(GL_VIEWPORT, view);
//	gluProject((GLdouble)x, (GLdouble)y, 0.0,
//		 model, proj, view,
//		 &pan_x, &pan_y, &pan_z);
//	gluUnProject((GLdouble)x, (GLdouble)y, pan_z,
//		 model, proj, view,
//		 &pan_x, &pan_y, &pan_z);
//	pan_y = -pan_y;

//	if (sphere_picked) {
//	  sphere_x = pan_x;
//	  sphere_y = pan_y;
//	  sphere_z = pan_z;
//	} else if (cone_picked) {
//	  cone_x = pan_x;
//	  cone_y = pan_y;
//	  cone_z = pan_z;
//	} else if (cube_picked) {
//	  cube_x = pan_x;
//	  cube_y = pan_y;
//	  cube_z = pan_z;
//	}
//  }

//  glutPostRedisplay();
//}

//void menu(int item)
//{
//	keyboard((unsigned char)item, 0, 0);
//}

//int main(int argc, char** argv)
//{
//	int ops, zoom;

//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

//	glutCreateWindow("CSG Operations Demo");

//	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	glutKeyboardFunc(keyboard);
//	glutMouseFunc(mouse);
//	glutMotionFunc(motion);

//	ops = glutCreateMenu(menu);
//	glutAddMenuEntry("A only          (a)", 'a');
//	glutAddMenuEntry("B only          (b)", 'b');
//	glutAddMenuEntry("A Or B          (|)", '|');
//	glutAddMenuEntry("A And B         (&)", '&');
//	glutAddMenuEntry("A sub B         (-)", '-');
//	glutAddMenuEntry("B sub A         (_)", '_');
//	zoom = glutCreateMenu(menu);
//	glutAddMenuEntry("Zoom decrease   (z)", 'z');
//	glutAddMenuEntry("Zoom increase   (Z)", 'Z');
//	glutCreateMenu(menu);
//	glutAddMenuEntry("CSG Operations Demo", '\0');
//	glutAddMenuEntry("                   ", '\0');
//	glutAddSubMenu(  "Operations         ", ops);
//	glutAddSubMenu(  "Zoom               ", zoom);
//	glutAddMenuEntry("                   ", '\0');
//	glutAddMenuEntry("Change shapes   (c)", 'c');
//	glutAddMenuEntry("                   ", '\0');
//	glutAddMenuEntry("Quit          (Esc)", '\033');
//	glutAttachMenu(GLUT_RIGHT_BUTTON);

//	init();

//	A = cube;
//	B = sphere;

//	glutMainLoop();

//	return 0;
//}
