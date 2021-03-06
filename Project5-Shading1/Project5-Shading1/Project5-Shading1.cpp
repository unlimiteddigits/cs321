/*	Shading Example #1
	Initially No light source and material color: Flat image
	+ : Increase ambient light component
	- : Decrease ambient light component	*/

#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>

static GLint width, height;
static GLfloat LR = -3.0;
static GLfloat theta = 0;

GLfloat vertices[] = {  -1.0,-2.0,-1.0,
						 1.0,-1.0,-1.0,
						 1.0, 1.0,-1.0, 
						-1.0, 1.0,-1.0, 
						-1.0,-1.0, 1.0,
						 1.0,-1.0, 1.0, 
						 1.0, 1.0, 1.0, 
						-1.0, 1.0, 1.0 };

GLfloat normals[] = {   -1.0,-1.0,-1.0,
						 1.0,-1.0,-1.0,
						 1.0, 1.0,-1.0, 
						-1.0, 1.0,-1.0, 
						-1.0,-1.0, 1.0,
						 1.0,-1.0, 1.0, 
						 1.0, 1.0, 1.0, 
						-1.0, 1.0, 1.0 };

GLfloat ambient_light[] = { 0.1, 0.1, 0.1, 1.0 };

void sleep(clock_t wait)
{
	clock_t goal;
	goal = wait + clock();
	while (goal > clock());
}

void other_init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void draw_cube(int v1, int v2, int v3, int v4)
{
	glBegin(GL_POLYGON);
	glNormal3fv(normals + v1 * 3);
	glVertex3fv(vertices + v1 * 3);
	glNormal3fv(normals + v2 * 3);
	glVertex3fv(vertices + v2 * 3);
	glNormal3fv(normals + v3 * 3);
	glVertex3fv(vertices + v3 * 3);
	glNormal3fv(normals + v4 * 3);
	glVertex3fv(vertices + v4 * 3);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);

	draw_cube(0, 3, 2, 1);	draw_cube(2, 3, 7, 6);
	draw_cube(0, 4, 7, 3);	draw_cube(1, 2, 6, 5);
	draw_cube(4, 5, 6, 7);	draw_cube(0, 1, 5, 4);
	glFlush();
	glutSwapBuffers();
}

void spinCube(void)
{
	theta += 5.0;
	if (theta > 360.0) theta -= 360.0;
	sleep((clock_t)(0.1 * CLOCKS_PER_SEC));
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		if (ambient_light[0] < 1.0)
		{
			ambient_light[0] += 0.1;
			ambient_light[1] += 0.1;
			ambient_light[2] += 0.1;
		}
		break;
	case '-':
		if (ambient_light[0] >= 0.1)
		{
			ambient_light[0] -= 0.1;
			ambient_light[1] -= 0.1;
			ambient_light[2] -= 0.1;
		}
		break;
	case 'Q':	case 'q': exit(1);
	}
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
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
	width = height = 500;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Shading Example 1");
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	other_init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(spinCube);
	glutReshapeFunc(reshape);
	glutMainLoop();
}

