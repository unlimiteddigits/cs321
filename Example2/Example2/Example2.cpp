/* Exmaple program for simple drawing */

#include <GL/freeglut.h>

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Simple Drawing");
}

void other_init()
{
	/* Set background */
	glClearColor(1.0, 1.0, 1.0, 1.0);

	/* Setup for viewing */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-500.0, 500.0, -500.0, 500.0, -500.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
	typedef GLfloat vertex3[3];
	int i;
	GLint x_offset, y_offset;

	vertex3 vertices1[3] = { {0.0,490.0,0.0},
							{-30.0,460.0,0.0},
							{30.0,460.0,0.0} };

	vertex3 vertices2[2] = { {-50.0, 400.0, 0.0},
							{50.0, 350.0, 0.0} };

	vertex3 vertices3[5] = { {0.0, 300.0, 0.0},
							{-50.0, 230.0, 0.0},
							{-50.0, 160.0, 0.0},
							{50.0, 160.0, 0.0},
							{50.0, 230.0, 0.0} };

	vertex3 vertices4[10] = { {-120.0, -100.0, 0.0},
							{-100.0, -200.0, 0.0},
							{-70.0, -100.0, 0.0},
							{-50.0, -200.0, 0.0},
							{-20.0, -100.0, 0.0},
							{0.0, -200.0, 0.0},
							{30.0, -100.0, 0.0},
							{50.0, -200.0, 0.0},
							{80.0, -100.0, 0.0},
							{100.0, -200.0, 0.0} };

	vertex3 vertices5[8] = { {0.0, -150.0, 0.0},
							{50.0, -100.0, 0.0},
							{100.0, -150.0, 0.0},
							{50.0, -200.0, 0.0},
							{-50.0, -200.0, 0.0},
							{-100.0, -150.0, 0.0},
							{-50.0, -100.0, 0.0},
							{50.0, -100.0, 0.0} };

	vertex3 vertices6[10] = { {-60.0, 0.0, 0.0},
							{-10.0, -200.0, 0.0},
							{60.0, 0.0, 0.0},
							{110.0, -200.0, 0.0},
							{180.0, 0.0, 0.0},
							{230.0, -200.0, 0.0},
							{300.0, 0.0, 0.0},
							{350.0, -200.0, 0.0} };

	GLfloat colors[][3] = { {0.0,0.0,0.0},{1.0,0.0,0.0},
	{0.0,1.0,0.0}, {0.0,0.0,1.0}, {1.0,1.0,0.0},
	{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0} };

	glClear(GL_COLOR_BUFFER_BIT);
	/* Set foreground color */
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(2.0);

	/* Draw a point */
	glBegin(GL_POINTS);
	glVertex3fv(vertices1[0]);
	glVertex3fv(vertices1[1]);
	glVertex3fv(vertices1[2]);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	/* Draw a line */
	glBegin(GL_LINES);
	glVertex3fv(vertices2[0]);
	glVertex3fv(vertices2[1]);
	glEnd();

	/* GL_LINE_STRIP, GL_LINE_LOOP, GL_POLYGON */
	glColor3f(1.0, 0.0, 0.0);
	x_offset = -200;
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < 5; i++)
		glVertex3f(vertices3[i][0] + x_offset, vertices3[i][1], 0.0);
	glEnd();

	x_offset = 0;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < 5; i++)
		glVertex3f(vertices3[i][0] + x_offset, vertices3[i][1], 0.0);
	glEnd();

	x_offset = 200;
	glBegin(GL_POLYGON);
	for (i = 0; i < 5; i++)
		glColor3fv(colors[i]);
	glVertex3f(vertices3[i][0] + x_offset, vertices3[i][1], 0.0);
	glEnd();

	/* Draw a triangle and rectangle */
	glColor3f(0.5, 0.5, 0.5);
	x_offset = -200;	y_offset = -450;
	glBegin(GL_TRIANGLES);
	for (i = 0; i < 3; i++)
		glVertex3f(vertices1[i][0] + x_offset, vertices1[i][1] + y_offset, 0.0);
	glEnd();

	y_offset = -150;
	glRectf(-50, 230 + y_offset, 50, 160 + y_offset);

	x_offset = 200;
	glBegin(GL_QUADS);
	for (i = 1; i < 5; i++)
		glVertex3f(vertices3[i][0] + x_offset, vertices3[i][1] + y_offset, 0.0);
	glEnd();

	/* GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUAD_STRIP */
	glColor3f(1.0, 0.0, 1.0);
	x_offset = -300;
	glBegin(GL_TRIANGLE_STRIP);
	for (i = 0; i < 10; i++)
		glVertex3f(vertices4[i][0] + x_offset, vertices4[i][1], 0.0);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	for (i = 0; i < 8; i++)
		glVertex3f(vertices5[i][0], vertices5[i][1], 0.0);
	glEnd();

	x_offset = 300;
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < 10; i++)
		glVertex3f(vertices4[i][0] + x_offset, vertices4[i][1], 0.0);
	glEnd();

	/* Draw Strips with color */
	x_offset = -300;	y_offset = -150;
	glBegin(GL_TRIANGLE_STRIP);
	for (i = 0; i < 10; i++)
	{
		glColor3fv(colors[i]);
		glVertex3f(vertices4[i][0] + x_offset, vertices4[i][1] + y_offset, 0.0);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	for (i = 0; i < 8; i++)
	{
		if ((i % 2) == 0) glColor3fv(colors[i]);
		glVertex3f(vertices5[i][0], vertices5[i][1] + y_offset, 0.0);
	}
	glEnd();

	x_offset = 300;
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < 10; i++)
	{
		if ((i % 2) == 0) glColor3fv(colors[i]);
		glVertex3f(vertices4[i][0] + x_offset, vertices4[i][1] + y_offset, 0.0);
	}
	glEnd();

	glFlush(); /* clear buffers */
}

void main(int argc, char** argv)
{
	/* Initialize window */
	init_window(argc, argv);

	/* Initialize other parameter */
	other_init();

	/* Redisplay callback event handling */
	glutDisplayFunc(display);

	/* Main event loop */
	glutMainLoop();
}