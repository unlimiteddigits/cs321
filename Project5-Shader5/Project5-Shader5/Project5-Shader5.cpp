/*	Shading Example #5
	Multiple light sources
	F, f: Disable light source
	1 : First light on
	2 : Second light on
	3 : Third light on						*/

//#include "glut.h"
#include <GL/freeglut.h>
#include <stdlib.h>

static GLint width, height;
static GLfloat LR = -3.0;

GLfloat light1_position[] = { 2.0, 0.0, 0.0, 1.0 };
GLfloat light2_position[] = { -2.0, 0.0, 0.0, 1.0 };
GLfloat light3_position[] = { 0.0, -2.0, 0.0, 1.0 };

GLfloat ambient_light[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat diffuse_light[] = { 0.7, 0.7, 0.7, 1.0 };

GLfloat light1_diffuse_light[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light2_diffuse_light[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat light3_diffuse_light[] = { 0.0, 0.0, 1.0, 1.0 };

void other_init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse_light);

	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse_light);

	glLoadIdentity();
	glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse_light);

	glLoadIdentity();
	glLightfv(GL_LIGHT2, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light3_diffuse_light);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(-1.2, 1.2, 0.0);
	glutSolidTeapot(0.8);
	glTranslatef(2.0, -2.0, 0.0);
	glutSolidSphere(0.8, 20, 16);
	glPopMatrix();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'F': 	case 'f':
		glDisable(GL_LIGHT0);		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);		break;
	case '1':
		glEnable(GL_LIGHT0);		break;
	case '2':
		glEnable(GL_LIGHT1);		break;
	case '3':
		glEnable(GL_LIGHT2);		break;
	case 'Q':	case 'q': exit(1);
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(LR, -LR, LR * (GLfloat)h / (GLfloat)w,
			-LR * (GLfloat)h / (GLfloat)w, LR, -LR);
	else
		glOrtho(LR * (GLfloat)w / (GLfloat)h,
			-LR * (GLfloat)w / (GLfloat)h, LR, -LR, LR, -LR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	width = w;	height = h;
}

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	width = height = 500;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Shading Example 5");
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	other_init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMainLoop();
}

