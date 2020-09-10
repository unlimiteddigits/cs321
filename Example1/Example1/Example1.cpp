// Example1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}
*/
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
/* Example program for Window initialization and drawing */

#include <GL/freeglut.h>

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);				/* Initialize GLUT */
	glutInitDisplayMode(GLUT_RGB); 		/* Specify display mode */

	glutInitWindowSize(200, 200);		/* Set window size */
	glutInitWindowPosition(0, 0);		/* Set window position */
	glutCreateWindow("Window Creation");	/* Create Window */
}

void other_init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);		/* Set background color */
	glMatrixMode(GL_PROJECTION);		/* Modify Projection Matrix */
	glLoadIdentity();				/* Set to identity matrix */
	glOrtho(0.0, 200.0, 200.0, 0.0, -1.0, 1.0);	/* Orthographic viewing volume */

//	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	/* Clear color values */
	glColor3f(0.0, 0.0, 1.0);			/* Set foreground color */
	glPointSize(4.0);				/* Set point size */

	/* Draw points */
	glBegin(GL_POINTS);
	glVertex3f(100, 30, 0);
	glVertex3f(70, 80, 0);
	glVertex3f(140, 80, 0);
	glEnd();

	glColor3f(1.0, 0.0, 1.0);
	glLineWidth(3.0);				/* Set line width */

	/* Draw a line */
	glBegin(GL_LINES);
	glVertex3f(50, 110, 0);
	glVertex3f(150, 190, 0);
	glEnd();
	glFlush();						/* Clear event buffer */
}

void main(int argc, char** argv)
{
	init_window(argc, argv);			/* Initialize window */
	other_init();						/* Initialize other parameter */
	glutDisplayFunc(display); 			/* Redisplay callback event handling */
	glutMainLoop();					/* Main event loop */
}
