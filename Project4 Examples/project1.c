/* Exmaple program for Orthographic Parallel Projection 

   X, Y, Z: Increase eye
   x, y, z: Decrease eye
 
   A, B, C: Increase look-at value
   a, b, c: Decrease look-at value
   
   D, E, F: Increase up vector
   d, e, f: Decrease up vector				*/
  
#include <GL/glut.h>
#include <stdio.h>

GLfloat width = 500;
GLfloat height  = 500; 

GLfloat Ex = 0.0, Ey = 0.0, Ez = 1.0, 
	    Ax = 0.0, Ay = 0.0, Az = 0.0, 
		Ux = 0.0, Uy = 1.0, Uz = 0.0;

typedef GLfloat vertex3[3];

// Vertex for Cubic box 
vertex3 cube1[8] = {{-0.3,-0.3,-0.3},{-0.1,-0.3,-0.3},
	{-0.1,-0.1,-0.3}, {-0.3,-0.1,-0.3}, {-0.3,-0.3,-0.1}, 
	{-0.1,-0.3,-0.1}, {-0.1,-0.1,-0.1}, {-0.3,-0.1,-0.1}};

vertex3 cube2[8] = {{0.1,0.1,0.1},{0.3,0.1,0.1},
	{0.3,0.3,0.1}, {0.1,0.3,0.1}, {0.1,0.1,0.3}, 
	{0.3,0.1,0.3}, {0.3,0.3,0.3}, {0.1,0.3,0.3}};

vertex3 cube3[8] = {{-0.5,-0.5,-0.5},{0.5,-0.5,-0.5},
	{0.5,0.5,-0.5}, {-0.5,0.5,-0.5}, {-0.5,-0.5,0.5}, 
	{0.5,-0.5,0.5}, {0.5,0.5,0.5}, {-0.5,0.5,0.5}};

vertex3 colors[6] = {{1.0,0.0,0.0},	{1.0,1.0,0.0}, 
	{0.0,1.0,0.0}, {0.0,0.0,1.0}, 
	{1.0,0.0,1.0}, {0.0,1.0,1.0}};


void init_window(int argc, char** argv)
{
    glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_RGB); 
	glutInitWindowSize(width, height); 
	glutInitWindowPosition(0,0); 
	glutCreateWindow("Projection"); 
	/* Set background */
	glClearColor(0.5, 0.5, 0.5, 1.0); 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 2.0);

}


/* Draw polygon with vertices list and color assign to vertices */
void draw_polygon(int a, int b, int c , int d, int n)
{
	/* Draw cube 1 and 2 */
	glColor3fv(colors[n]);
	glBegin(GL_POLYGON);
		glVertex3fv(cube1[a]);
		glVertex3fv(cube1[b]);
		glVertex3fv(cube1[c]);
		glVertex3fv(cube1[d]);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3fv(cube2[a]);
		glVertex3fv(cube2[b]);
		glVertex3fv(cube2[c]);
		glVertex3fv(cube2[d]);
	glEnd();
}

void draw_cube()
{
	draw_polygon(0,3,2,1,0);
	draw_polygon(2,3,7,6,1);
	draw_polygon(0,4,7,3,2);
	draw_polygon(1,2,6,5,3);
	draw_polygon(4,5,6,7,4);
	draw_polygon(0,1,5,4,5);
}

void draw_axis_polygon(int a, int b, int c , int d, int n)
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
	draw_axis_polygon(0,3,2,1,0);
	draw_axis_polygon(2,3,7,6,1);
	draw_axis_polygon(0,4,7,3,2);
	draw_axis_polygon(1,2,6,5,3);
	draw_axis_polygon(4,5,6,7,4);
	draw_axis_polygon(0,1,5,4,5);
}

void change_view()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Ex, Ey, Ez, Ax, Ay, Az, Ux, Uy, Uz);
}

void display(void)
{
	change_view();
	/* clear buffers to preset values.  */
	glClear(GL_COLOR_BUFFER_BIT);
	draw_axis();
	draw_cube();
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
	change_view();
	display();
}


void main(int argc, char** argv)
{
	/* Initialize window */
    init_window(argc,argv);
	/* Redisplay callback event handling */
    glutDisplayFunc(display); 
	/* Key event handler */
	glutKeyboardFunc(keyboard);
	/* Main event loop */
    glutMainLoop(); 
}




