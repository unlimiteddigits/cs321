/* Example program #1
   No back face culling */

#include <stdlib.h>
#include <GL/freeglut.h>

static GLint width, height;
static GLfloat LR = 3.0, TB = 3.0, NZ = -3.0, FZ = 3.0;
GLint Ax = 0, Ay = 0, Az = 0;

GLfloat vertices[] = { -1.0,-1.0,-1.0,1.0,-1.0,-1.0,
		1.0,1.0,-1.0, -1.0,1.0,-1.0, -1.0,-1.0,1.0,
		1.0,-1.0,1.0, 1.0,1.0,1.0, -1.0,1.0,1.0 };

GLfloat colors[] = { 0.0,0.0,0.5, 0.0,1.0,0.0,
					1.0,0.0,0.0, 0.5,0.0,0.0,
					0.0,0.0,1.0, 0.0,0.5,0.0 };

void draw_cube(int v1, int v2, int v3, int v4, int index)
{
	glBegin(GL_POLYGON);
	glColor3fv(colors + index * 3);
	glVertex3fv(vertices + v1 * 3);
	glVertex3fv(vertices + v2 * 3);
	glVertex3fv(vertices + v3 * 3);
	glVertex3fv(vertices + v4 * 3);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(Ax, 1.0, 0.0, 0.0);
	glRotatef(Ay, 0.0, 1.0, 0.0);
	glRotatef(Az, 0.0, 0.0, 1.0);

	draw_cube(0, 3, 2, 1, 0);
	draw_cube(2, 3, 7, 6, 1);
	draw_cube(0, 4, 7, 3, 2);
	draw_cube(1, 2, 6, 5, 3);
	draw_cube(4, 5, 6, 7, 4);
	draw_cube(0, 1, 5, 4, 5);

	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-LR, LR, -TB * (GLfloat)h / (GLfloat)w,
			TB * (GLfloat)h / (GLfloat)w, NZ, FZ);
	else
		glOrtho(-LR * (GLfloat)w / (GLfloat)h,
			LR * (GLfloat)w / (GLfloat)h, -TB, TB, NZ, FZ);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	width = w;	height = h;
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'X' || key == 'x')	Ax += 10.0;
	else if (key == 'Y' || key == 'y')	Ay += 10.0;
	else if (key == 'Z' || key == 'z')	Az += 10.0;
	else if (key == 'L' || key == 'l')
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (key == 'F' || key == 'f')
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (key == 'Q' || key == 'q')	exit(1);
	glutPostRedisplay();
}

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	width = height = 500;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Back Face Culling Example");
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glColor3f(1.0, 1.0, 1.0);
	glutMainLoop();
}


