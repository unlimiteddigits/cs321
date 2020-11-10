/*	Example for lighting #7
	S, s: Add specular light and specular properties of material
	+: Increase the shininess
	-: decrease the shininess								*/

#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

static GLfloat LR = -3.0;
static GLint width, height;

// Setting for GL_LIGHT0
GLfloat  ambient_light[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat  diffuse_light[] = { 1.0f, 0.4f, 0.4f, 1.0f };
GLfloat  specular_light[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  specular_property[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  light_position[] = { 0.0, 0.0, 3.0, 1.0 };
GLint	 shininess = 5;

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

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, shininess);
	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
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

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'S': 	case 's':
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_property);
		break;
	case '+':
		if (shininess < 126) shininess += 2;
		glMateriali(GL_FRONT, GL_SHININESS, shininess);	break;
	case '-':
		if (shininess > 2) shininess -= 2;
		glMateriali(GL_FRONT, GL_SHININESS, shininess);	break;
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
	width = height = 600;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Shading Exmaple 7");
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	set_up_light();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(spinCube);
	glutMainLoop();
}

