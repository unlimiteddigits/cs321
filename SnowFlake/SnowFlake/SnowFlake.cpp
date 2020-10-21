/* Exmaple program for Push and Pop matrix */

#include <GL/freeglut.h>

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);					// glutInit will initialize the GLUT library and negotiate a session with the window system. During this process, glutInit may cause the termina- tion of the GLUT program with an error message to the user if GLUT can- not be properly initialized. Examples of this situation include the failure to connect to the window system, the lack of window system sup- port for OpenGL, and invalid command line options.
	glutInitDisplayMode(GLUT_RGB);			//The initial display mode is used when creating top-level windows, sub- windows, and overlays to determine the OpenGL display mode for the to-be-created window or overlay.
	glutInitWindowSize(500, 500);			//
	glutInitWindowPosition(10, 100);		//Windows created by glutCreateWindow will be requested to be created with the current initial window position and size.
	glutCreateWindow("Matrix Push & Pop");	//glutCreateWindow creates a top-level window. The name will be provided to the window system as the window's name. The intent is that the win- dow system will label the window with the name.
}

void other_init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);		//
	glColor3f(0.8, 0.8, 0.8);				//
	glPointSize(1.1);						//specifies the rasterized diameter of both aliased and antialiased points
	glMatrixMode(GL_PROJECTION);			//sets the current matrix mode. mode can assume one of four values: Applies subsequent matrix operations to ...  GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE, or GL_COLOR
	glLoadIdentity();						//replaces the current matrix with the identity matrix. 
	glOrtho(0.0, 500.0, 0.0, 500.0, -1.0, 1.0);//describes a transformation that produces a parallel projection. The current matrix (see glMatrixMode) is multiplied by this matrix and the result replaces the current matrix
	glMatrixMode(GL_MODELVIEW);				//
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
	for (i = 0; i < 3; i++)
	{
		glPushMatrix();  // Containes all previous geometric transformation
		x_coord = rand() % 500;	y_coord = rand() % 500;		// Location
		glTranslated(x_coord, y_coord, 0);					// Size
		size = define_scale_factor();
		glScaled(size*2, size*2, 1.0);							//
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