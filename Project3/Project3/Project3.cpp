/* Exmaple program for Push and Pop matrix */

#include <GL/freeglut.h>

typedef GLfloat vertex3[3];
vertex3 vertices1[5] = {
						{0.0, 0.0, 0.0},
						{0.0, 50.0, 0.0},
						{25.0, 80.0, 0.0},
						{50.0, 50.0, 0.0},
						{50.0, 0.0, 0.0} };

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Matrix Push & Pop");
}

void other_init()
{
	/* Set background */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(1.0, 0.0, 1.0);

	/* Setup for viewing */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 500.0, 0.0, 500.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void draw_barn()
{
	int i;

	glBegin(GL_LINE_LOOP);
	for (i = 0; i < 5; i++)
		glVertex3f(vertices1[i][0], vertices1[i][1], 0.0);
	glEnd();
}

float define_scale_factor()
{
	int temp;
	GLfloat scale;

	temp = rand() % 3;

	if (temp == 0) scale = 0.33;
	else if (temp == 1) scale = 0.66;
	else scale = 1.0;

	return scale;
}

void display(void)
{
	GLint x_offset, y_offset;

	int row, column, i, j;
	row = 12; column = 12;

	x_offset = 5;	y_offset = 5;
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();  // Containes all previous geometric transformation
	glTranslated(x_offset, y_offset, 0);

	for (i = 0; i < row; i++)
	{
		glPushMatrix();
		for (j = 0; j < column; j++)
		{
			glPushMatrix();
			glScaled(define_scale_factor(), define_scale_factor(), 1.0);
			draw_barn();
			glPopMatrix();
			glTranslated(x_offset * 12, 0, 0);
		}
		glPopMatrix();
		glTranslated(0, y_offset * 20, 0);
	}
	glPopMatrix();
	glFlush(); /* clear buffers */
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	other_init();
	glutDisplayFunc(display);
	glutMainLoop();
}