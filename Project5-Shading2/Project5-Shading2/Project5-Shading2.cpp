/*	Shading Example #2
Without Light Source with material color using material properties
	E, e: Emission light on
*/

#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>

static GLint width, height;
static GLfloat LR = -2.0;
static GLfloat theta = 0;

GLfloat vertices[] = {  -1.0,-1.0,-1.0,    /* 0 */
						 1.0,-1.0,-1.0,    /* 1 */
						 1.0, 1.0,-1.0,    /* 2 */
						-1.0, 1.0,-1.0,    /* 3 */
						-1.0,-1.0, 1.0,    /* 4 */
						 1.0,-1.0, 1.0,    /* 5 */
						 1.0, 1.0, 1.0,    /* 6 */
						-1.0, 1.0, 1.0 };  /* 7 */

GLfloat colors[] = { 0.9, 0.1, 0.1,			/*  back side - Red */
					0.935, 0.35, 0.935,		/* top side - purple*/
					0.05, 0.95, 0.05,		/* Left Side - green */
					0.065, 0.065, 0.95,		/* Right Side - blue */
					.992, 1, 0,				/* Front Side - yellow */
					0.995, 0.995, 0.995 };  /* Bottom Side - White */

GLfloat normals[] = { -1.0,-1.0,-1.0, 
						1.0,-1.0,-1.0,
						1.0,1.0,-1.0, 
						-1.0,1.0,-1.0, 
						-1.0,-1.0,1.0,
						1.0,-1.0,1.0, 
						1.0,1.0,1.0,  
						-1.0,1.0,1.0 };

GLfloat ambient_light[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat emission_light[] = { 0.5, 0.5, 1.0, 1.0 };

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
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void draw_cube(int v1, int v2, int v3, int v4, int color)
{
	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + color * 3);

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
	glRotatef(-theta, 0.0, 0.0, 1.0);

	draw_cube(0, 3, 2, 1, 0);	draw_cube(2, 3, 7, 6, 1);
	draw_cube(0, 4, 7, 3, 2);	draw_cube(1, 2, 6, 5, 3);
	/*draw_cube(4, 5, 6, 7, 4);	draw_cube(0, 1, 5, 4, 5);*/
	glFlush();
	glutSwapBuffers();
}

void spinCube(void)
{
	theta += 01.0;
	if (theta > 360.0) theta -= 360.0;
	sleep((clock_t)(0.1 * CLOCKS_PER_SEC));
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'E':	case 'e':
		glMaterialfv(GL_FRONT, GL_EMISSION,
			emission_light);
		break;
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	width = height = 500;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Shading Example 2");
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

