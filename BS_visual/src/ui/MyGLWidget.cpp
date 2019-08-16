#include "MyGLWidget.h"

#include "GL/glut.h"
#include "GL/glu.h"

MyGLWidget* g_CurrentInstance;

void drawCallback()
{
	g_CurrentInstance->draw();
}

void resizeCallback(int w, int h)
{
	g_CurrentInstance->resize(w, h);
}

void rotateCallback(int button, int state, int x, int y)
{
	g_CurrentInstance->resize(w, h);
}

MyGLWidget::MyGLWidget(const std::string &filename)
{
	m_particle = new Particle;
	m_particle->SetFromFile(filename);
}

MyGLWidget::~MyGLWidget()
{
//	makeCurrent();
	delete scene;

//	delete m_texture;
//	delete m_shader;
//	delete m_program;

//	m_vbo.destroy();
//	m_vao.destroy();
	//	doneCurrent();
}

void MyGLWidget::display()
{
	char *argv[] = {"Particle"};
	int argc = 1;
	glutInit(&argc, argv);
	glutInitDisplayMode(/*GLUT_SINGLE*/GLUT_DOUBLE /*| GLUT_RGB*/);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Experiment");
	g_CurrentInstance = this;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	::glutDisplayFunc(::drawCallback);
	::glutReshapeFunc(::resizeCallback);
	::glutMouseFunc(::rotateCallback);
//	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH/*GL_FLAT*/);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

	glMatrixMode(GL_PROJECTION);
	glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 3.0F);

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0F, 0.0F, -2.0F);

	glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
	glRotatef(30.0F, 0.0F, 1.0F, 0.0F);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutMainLoop();
}

void MyGLWidget::resize(int w, int h)
{
	// Calculate aspect ratio
	qreal aspect = qreal(w) / qreal(h ? h : 1);

	glViewport(0, 0, w, h);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
	// Reset projection
//	projection.setToIdentity();

	// Set perspective projection
//	projection.perspective(45.0, aspect, 0.01f, 100.0f);
	// Compute aspect ratio of the new window
//	if (h == 0) h = 1;                // To prevent divide by 0
//	GLfloat aspect = (GLfloat)w / (GLfloat)h;

//	// Set the viewport to cover the new window
//	glViewport(0, 0, w, h);

//	// Set the aspect ratio of the clipping volume to match the viewport
//	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
//	glLoadIdentity();             // Reset
//	// Enable perspective projection with fovy, aspect, zNear and zFar
//	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void MyGLWidget::draw()
{
////	makeCurrent();
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////	glDepthFunc(GL_LESS);
////	glEnable(GL_DEPTH_TEST);
////	glShadeModel(GL_FLAT);
////	glMatrixMode(GL_PROJECTION);

////	texture->bind();

//	// Calculate model view transformation
//	QMatrix4x4 matrix;
//	matrix.translate(0.0, 0.0, -5.0);
//	matrix.rotate(rotation);

//	program.setUniformValue("mvp_matrix", projection * matrix);
//	program.setUniformValue("texture", 0);

////	scene->drawGeometry(&program);

//	// Set modelview-projection matrix
////	program.setUniformValue("mvp_matrix", projection * matrix);

////	glLoadIdentity();

////	glBegin(GL_POLYGON);

////	glColor3f(0.0, 0.0, 1.0);
////	glLineWidth(50); //задаем толщину
////	glColor3ub(60, 170, 60);
////	glBegin(GL_LINES);
////	glVertex3f(-9, 0.0, 0.0);
////	glVertex3f(9, 0, 0);

////	glColor3f(1.0, 0.0, 0.0 );
////	glVertex3f(500.0f, 500.0f, 500.0f);
////	glVertex3f(500.0f, -500.0f, 500.0f);
////	glVertex3f(-500.0f, 500.0f, 500.0f);
////	glVertex3f(-500.0f, -500.0f, 500.0f);

////	glEnd();
////	glClearColor(1, 0, 0, 0);
////	drawFacets();

////	glFlush();
////	swapBuffers();
////	doneCurrent();
////	this->
//	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

//	Point3f lightSourcePoint(1, 1, 1);

	for (int i = 0; i < m_particle->nElems; ++i)
	{
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -7.0f);
		glBegin(GL_POLYGON);
		glColor3f(0.0f, 0.0f, 1.0f);// blue
//		glColor3f(0.6f, 0.6f, 0.6f);// grey

		Facet &f = m_particle->elems[i].actual;
		Point3f &n = f.ex_normal;
		glNormal3f(n.coordinates[0], n.coordinates[1], n.coordinates[2]);

		for (int j = 0; j < f.nVertices; ++j)
		{
			Point3f p = f.arr[j];
			Point3f::Normalize(p);
//			Point3f shadeVector = lightSourcePoint - p;
//			Point3f::Normalize(shadeVector);
//			float shadeValue = Point3f::Length(shadeVector)/2;
//			glColor3f(shadeValue, shadeValue, shadeValue);// grey
			p = p * 2;
			glVertex3f(p.coordinates[0], p.coordinates[1], p.coordinates[2]);
		}

		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 0.0f, 0.0f);// black
		glLineWidth(20);

		for (int j = 0; j < f.nVertices; ++j)
		{
			Point3f &p = f.arr[j];
			Point3f::Normalize(p);
			p = p * 2;
			glVertex3f(p.coordinates[0], p.coordinates[1], p.coordinates[2]);
		}

		glEnd();
	}

	// Render a color-cube consisting of 6 quads with different colors
//	glLoadIdentity();                 // Reset the model-view matrix
//	glTranslatef(1.5f, 0.0f, -7.0f);  // Move right and into the screen

//	glBegin(GL_POLYGON);                // Begin drawing the color cube with 6 quads
//	   // Top face (y = 1.0f)
//	   // Define vertices in counter-clockwise (CCW) order with normal pointing out
//	   glColor3f(0.0f, 1.0f, 0.0f);     // Green
//	   glVertex3f( 1.0f, 1.0f, -1.0f);
//	   glVertex3f(-1.0f, 1.0f, -1.0f);
//	   glVertex3f(-1.0f, 1.0f,  1.0f);
//	   glVertex3f( 1.0f, 1.0f,  1.0f);

//	   // Bottom face (y = -1.0f)
//	   glColor3f(1.0f, 0.5f, 0.0f);     // Orange
//	   glVertex3f( 1.0f, -1.0f,  1.0f);
//	   glVertex3f(-1.0f, -1.0f,  1.0f);
//	   glVertex3f(-1.0f, -1.0f, -1.0f);
//	   glVertex3f( 1.0f, -1.0f, -1.0f);

//	   // Front face  (z = 1.0f)
//	   glColor3f(1.0f, 0.0f, 0.0f);     // Red
//	   glVertex3f( 1.0f,  1.0f, 1.0f);
//	   glVertex3f(-1.0f,  1.0f, 1.0f);
//	   glVertex3f(-1.0f, -1.0f, 1.0f);
//	   glVertex3f( 1.0f, -1.0f, 1.0f);

//	   // Back face (z = -1.0f)
//	   glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
//	   glVertex3f( 1.0f, -1.0f, -1.0f);
//	   glVertex3f(-1.0f, -1.0f, -1.0f);
//	   glVertex3f(-1.0f,  1.0f, -1.0f);
//	   glVertex3f( 1.0f,  1.0f, -1.0f);

//	   // Left face (x = -1.0f)
//	   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
//	   glVertex3f(-1.0f,  1.0f,  1.0f);
//	   glVertex3f(-1.0f,  1.0f, -1.0f);
//	   glVertex3f(-1.0f, -1.0f, -1.0f);
//	   glVertex3f(-1.0f, -1.0f,  1.0f);

//	   // Right face (x = 1.0f)
//	   glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
//	   glVertex3f(1.0f,  1.0f, -1.0f);
//	   glVertex3f(1.0f,  1.0f,  1.0f);
//	   glVertex3f(1.0f, -1.0f,  1.0f);
//	   glVertex3f(1.0f, -1.0f, -1.0f);
//	glEnd();  // End of drawing color-cube

//	// Render a pyramid consists of 4 triangles
//	glLoadIdentity();                  // Reset the model-view matrix
//	glTranslatef(-1.5f, 0.0f, -6.0f);  // Move left and into the screen

//	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
//	   // Front
//	   glColor3f(1.0f, 0.0f, 0.0f);     // Red
//	   glVertex3f( 0.0f, 1.0f, 0.0f);
//	   glColor3f(0.0f, 1.0f, 0.0f);     // Green
//	   glVertex3f(-1.0f, -1.0f, 1.0f);
//	   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
//	   glVertex3f(1.0f, -1.0f, 1.0f);

//	   // Right
//	   glColor3f(1.0f, 0.0f, 0.0f);     // Red
//	   glVertex3f(0.0f, 1.0f, 0.0f);
//	   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
//	   glVertex3f(1.0f, -1.0f, 1.0f);
//	   glColor3f(0.0f, 1.0f, 0.0f);     // Green
//	   glVertex3f(1.0f, -1.0f, -1.0f);

//	   // Back
//	   glColor3f(1.0f, 0.0f, 0.0f);     // Red
//	   glVertex3f(0.0f, 1.0f, 0.0f);
//	   glColor3f(0.0f, 1.0f, 0.0f);     // Green
//	   glVertex3f(1.0f, -1.0f, -1.0f);
//	   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
//	   glVertex3f(-1.0f, -1.0f, -1.0f);

//	   // Left
//	   glColor3f(1.0f,0.0f,0.0f);       // Red
//	   glVertex3f( 0.0f, 1.0f, 0.0f);
//	   glColor3f(0.0f,0.0f,1.0f);       // Blue
//	   glVertex3f(-1.0f,-1.0f,-1.0f);
//	   glColor3f(0.0f,1.0f,0.0f);       // Green
//	   glVertex3f(-1.0f,-1.0f, 1.0f);
//	glEnd();   // Done drawing the pyramid

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
//	swapBuffers();
}
