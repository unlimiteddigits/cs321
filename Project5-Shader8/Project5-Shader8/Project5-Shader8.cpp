/*	Example for lighting #8
	1: Add Spot Light1
	2: Add Spot Light2
	I, i: Increase the cutoff angle
	D, d: Decrease the cutoff angle
	O, o: Off spot light
	+: Increase the focus (exponent)
	-: Decrease the focus (exponent)		*/

#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>

static GLfloat LR = -3.0;
static GLint width, height;

// Setting for GL_LIGHT0
GLfloat  ambient_light[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat  diffuse_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat  specular_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat  specular_property[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat  light_position[] = { 0.0, 5.0, 0.0, 1.0 };
GLint	 shininess = 40;

// Setting for GL_LIGHT1
// Green Spotlight with narrow angle
GLfloat diffuse_light1[] = { 0.2, 0.4, 0.9, 1.0 };
GLfloat light_position1[] = { -0.3, 5.0, -0.3, 1.0 };
GLfloat spot_direction1[] = { -0.3, -2.0, -0.3 };

// Setting for GL_LIGHT2
// Red Spotlight with wide angle
GLfloat diffuse_light2[] = { 1.0, 0.2, 0.6, 1.0 };
GLfloat light_position2[] = { 0.3, -0.5, 4.0, 1.0 };
GLfloat spot_direction2[] = { 0.3, -0.5, -2.0 };

GLint	 exponent = 0;
GLfloat	 cutoff = 10.0;


void set_up_light(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);

	// Spot Light 1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);

	glLoadIdentity();
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	glLoadIdentity();
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
	glLighti(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
	glEnable(GL_LIGHT1);

	// Spot Light 2
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse_light2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular_light);

	glLoadIdentity();
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, cutoff);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2);

	glLighti(GL_LIGHT2, GL_SPOT_EXPONENT, exponent);
	glEnable(GL_LIGHT2);

	// Set Material properties to follow glColor values
	glMateriali(GL_FRONT, GL_SHININESS, shininess);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(20.0, 1.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef(-0.75, 0.5, -0.5);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidTorus(0.275, 0.85, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, -0.5, -0.5);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	glutSolidCone(1.0, 2.0, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0, -1.0, 0.0);
	glutSolidSphere(1.0, 15, 15);
	glPopMatrix();

	glPopMatrix();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		glEnable(GL_LIGHT1);					break;
	case '2':
		glEnable(GL_LIGHT2);					break;
	case 'I': 	case 'i':
		if (cutoff >= 90.0) cutoff = 180.0;
		else if ((cutoff < 90.0) && (cutoff >= 1))cutoff += 4.0;
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, cutoff);
		break;
	case 'D': 	case 'd':
		if (cutoff >= 180.0) cutoff = 89.0;
		else if (cutoff >= 5.0) cutoff -= 4.0;
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, cutoff);
		break;
	case '+':
		if (exponent < 100) exponent += 5;
		glLighti(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
		glLighti(GL_LIGHT2, GL_SPOT_EXPONENT, exponent);
		break;
	case '-':
		if (exponent >= 5) exponent -= 5;
		glLighti(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
		glLighti(GL_LIGHT2, GL_SPOT_EXPONENT, exponent);
		break;
	case 'O':	case 'o':
		glDisable(GL_LIGHT1);	glDisable(GL_LIGHT2);	break;
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
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	width = height = 600;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Shading Exmaple 8");
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	set_up_light();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

