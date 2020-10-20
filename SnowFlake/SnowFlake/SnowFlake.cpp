/* Exmaple program for Push and Pop matrix */

#include <GL/freeglut.h>

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(10, 100);
	glutCreateWindow("Matrix Push & Pop");
}

void other_init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.8, 0.8, 0.8);
	glPointSize(.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 500.0, 0.0, 500.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void draw_one_snowflake()
{
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(20, 0, 0);
	glVertex3i(8, 0, 0);
	glVertex3i(12, 6, 0);
	glVertex3i(14, 0, 0);
	glVertex3i(18, 4, 0);
	glEnd();
}

void draw_both_snowflake()
{
	draw_one_snowflake();
	glPushMatrix();
	glScaled(1.0, -1.0, 1.0);
	draw_one_snowflake();
	glPopMatrix();
}

void draw_upper_side_snowflake()
{
	glPushMatrix();
	draw_one_snowflake();
	glRotated(60.0, 0, 0, 1);
	draw_both_snowflake();
	glRotated(60.0, 0, 0, 1);
	draw_both_snowflake();
	glRotated(60.0, 0, 0, 1);
	draw_one_snowflake();
	glPopMatrix();
}

void draw_snowflake()
{
	glPushMatrix();
	draw_upper_side_snowflake();
	glScaled(1.0, -1.0, 1.0);
	draw_upper_side_snowflake();
	glPopMatrix();
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
	int x_coord, y_coord, i;
	float size;

	glClear(GL_COLOR_BUFFER_BIT);
	for (i = 0; i < 200; i++)
	{
		glPushMatrix();  // Containes all previous geometric transformation
		x_coord = rand() % 500;	y_coord = rand() % 500;		// Location
		glTranslated(x_coord, y_coord, 0);					// Size
		size = define_scale_factor();
		glScaled(size, size, 1.0);
		draw_snowflake();
		glPopMatrix();
	}
	glFlush();
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	other_init();
	glutDisplayFunc(display);
	glutMainLoop();
}