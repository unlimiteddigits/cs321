/*	Example for lighting #6
	Rotating object, stationary light source */

#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

static GLfloat LR = -3.0;
static GLint width, height;

// Setting for GL_LIGHT0
GLfloat diffuse_light0[] = { 0.4, 0.4, 1.0, 1.0 };
GLfloat light_position0[] = { 0.0, 0.0, 3.0, 1.0 };

void sleep(clock_t wait)
{
	clock_t goal;
	goal = wait + clock();
	while (goal > clock());
}

void set_up_light(void)
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotated(5.0, 0.0, 1.0, 0.0);
	glutSolidTeapot(1.0);
	glFlush();
	glutSwapBuffers();
}

void spinCube(void)
{
	sleep((clock_t)(0.1 * CLOCKS_PER_SEC));
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	width = height = 600;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Shading Exmaple 6");
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	set_up_light();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(spinCube);
	glutMainLoop();
}

