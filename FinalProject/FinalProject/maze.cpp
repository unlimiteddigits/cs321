#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include "FinalProject.h"
GLfloat vertices[] = { -1.0,-1.0,-1.0,    /* 0 */
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


int squares[] = { 12,9,13,12,10,10,10,10,10,10,3,14,10,10,9,
						5,5,5,5,12,10,9,12,10,10,10,8,10,10,1,
						5,6,3,5,5,13,5,6,11,12,9,5,13,12,3,
						4,10,10,3,5,5,6,10,9,5,5,5,5,5,13,
						5,12,10,10,1,4,10,9,6,3,6,3,5,6,1,
						5,6,10,11,5,5,14,2,10,9,14,9,4,10,3,
						6,10,10,10,3,4,10,10,9,6,9,5,5,14,9,
						12,10,10,10,10,3,13,12,3,14,2,3,6,10,1,
						5,14,8,9,12,9,5,5,12,10,10,10,10,10,3,
						6,10,3,6,3,6,1,6,2,10,10,10,10,10,11
					 };
void mazeFloor() {
	char X_Label[] = "X-Axis";
	char Y_Label[] = "Y-Axis";
	char Z_Label[] = "Z-Axis";

	//glEnable(GL_LINES);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	//glEnable(GL_LINES);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 0.1f, 0.0f);
	glVertex3f(0.1f, 0.1f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();

	GLfloat sqSize = .05;
	GLfloat sqBorder = .008;
	int mazeWidth = 15;
	int mazeHeight = 10;
	int row = 0, column = 0;
	int blockNumber = 0;
	GLfloat xll, xlr, xul, xur;
	GLfloat yll, ylr, yul, yur;
	GLfloat	increment;
	increment = sqBorder + sqSize + sqBorder;

	for (row = 0; row < mazeHeight; row++){
		for (column = 0; column < mazeWidth; column++) {
			blockNumber = row * mazeWidth + column;
			xll = (increment)*column;
			yll = (increment)*row;
			xlr = increment + (increment)*column;
			ylr = (increment)*row;
			xur = increment + (increment)*column;
			yur = increment + (increment)*row;
			xul = (increment)*column;
			yul = increment + (increment)*row;

			if (squares[blockNumber] & 1) {
				glBegin(GL_LINES);
				glVertex3f(xlr, ylr, 0);
				glVertex3f(xur, yur, 0);
				glEnd();
			}
			if (squares[blockNumber] & 2) {
				glBegin(GL_LINES);
				glVertex3f(xur, yur, 0);
				glVertex3f(xul, yul, 0);
				glEnd();
			}
			if (squares[blockNumber] & 4) {
				glBegin(GL_LINES);
				glVertex3f(xul, yul, 0);
				glVertex3f(xll, yll, 0);
				glEnd();
			}
			if (squares[blockNumber] & 8) {
				glBegin(GL_LINES);
				glVertex3f(xll, yll, 0);
				glVertex3f(xlr, ylr, 0);
				glEnd();
			}
		}
	}
}