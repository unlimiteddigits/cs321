/* Exmaple program for Perspective Projection
   With glut drawing objects

   X, Y, Z: Increase eye
   x, y, z: Decrease eye

   A, B, C: Increase look-at value
   a, b, c: Decrease look-at value

   D, E, F: Increase up vector
   d, e, f: Decrease up vector				*/

#include <GL/glut.h>
#include <stdio.h>

GLfloat width = 500;
GLfloat height = 500;

GLfloat Ex = 0.0, Ey = 0.0, Ez = 3.0,
Ax = 0.0, Ay = 0.0, Az = 0.0,
Ux = 0.0, Uy = 1.0, Uz = 0.0;

typedef GLfloat vertex3[3];

vertex3 cube3[8] = { {-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
	{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0},
	{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0} };

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Projection");
	/* Set background */
	glClearColor(0.5, 0.5, 0.5, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.1, 1.1, -1.1, 1.1, 1.0, 4.0);
}

void draw_axis_polygon(int a, int b, int c, int d, int n)
{
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(cube3[a]);
	glVertex3fv(cube3[b]);
	glVertex3fv(cube3[c]);
	glVertex3fv(cube3[d]);
	glEnd();
}

/* Draw axis box */
void draw_axis()
{
	draw_axis_polygon(0, 3, 2, 1, 0);
	draw_axis_polygon(2, 3, 7, 6, 1);
	draw_axis_polygon(0, 4, 7, 3, 2);
	draw_axis_polygon(1, 2, 6, 5, 3);
	draw_axis_polygon(4, 5, 6, 7, 4);
	draw_axis_polygon(0, 1, 5, 4, 5);
}

void display(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Ex, Ey, Ez, Ax, Ay, Az, Ux, Uy, Uz);

	/* clear buffers to preset values.  */
	glClear(GL_COLOR_BUFFER_BIT);
	draw_axis();

	glColor3f(0.0, 1.0, 1.0);
	glutWireTorus(0.1, 0.2, 10, 10);

	glColor3f(0.0, 0.9, 0.0);
	glTranslated(0.6, 0.6, 0.4);
	glutWireTeapot(0.25);

	glColor3f(0.0, 0.0, 0.9);
	glTranslated(-1.2, -1.2, -0.8);
	glutWireTeapot(0.25);

	glColor3f(0.9, 0.0, 0.0);
	glTranslated(0.0, 1.2, 1.2);
	glutWireSphere(0.3, 30, 30);

	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'X': Ex += 0.2;	break;
	case 'Y': Ey += 0.2;	break;
	case 'Z': Ez += 0.2;	break;
	case 'x': Ex -= 0.2;	break;
	case 'y': Ey -= 0.2;	break;
	case 'z': Ez -= 0.2;	break;

	case 'A': Ax += 0.2;	break;
	case 'B': Ay += 0.2;	break;
	case 'C': Az += 0.2;	break;
	case 'a': Ax -= 0.2;	break;
	case 'b': Ay -= 0.2;	break;
	case 'c': Az -= 0.2;	break;

	case 'D': Ux += 1;	break;
	case 'E': Uy += 1;	break;
	case 'F': Uz += 1;	break;
	case 'd': Ux -= 1;	break;
	case 'e': Uy -= 1;	break;
	case 'f': Uz -= 1;	break;

	case 'Q':
	case 'q': exit(1);
	}
	display();
}


void main(int argc, char** argv)
{
	/* Initialize window */
	init_window(argc, argv);
	/* Redisplay callback event handling */
	glutDisplayFunc(display);
	/* Key event handler */
	glutKeyboardFunc(keyboard);
	/* Main event loop */
	glutMainLoop();
}




