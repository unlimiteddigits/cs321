#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include "FinalProject.h"

GLfloat sqSize = .05;
GLfloat sqBorder = .008;
int mazeWidth = 15;
int mazeHeight = 10;
int row = 0, column = 0;
int blockNumber = 0;
int GridPosX = 11, GridPosY = 0;

/* Left, back, bottom*/
/*Right, Front, top*/
GLfloat vertices[] = { 0.000, 0.000, 0.001,    /* 0 - Back Bottom	Left */
						0.066, 0.000, 0.001,    /* 1 - Back Bottom	Right*/
						0.066, 0.008, 0.001,    /* 2 - Back Top		Right*/
						0.000, 0.008, 0.001,    /* 3 - Back Top		Left */
						0.000, 0.000, 0.066,    /* 4 - Front Bottom	Left */
						0.066, 0.000, 0.066,    /* 5 - Front Bottom	Right*/
						0.066, 0.008, 0.066,    /* 6 - Front Top		Right*/
						0.000, 0.008, 0.066 };  /* 7 - Front Top		Left*/

					  /* Left, back, bottom*/
					  /*Right, Front, top*/
GLfloat vertices2[] = { 0.000, 0.008, 0.001,    /* 0 - Back Bottom	Left */
						0.058, 0.008, 0.001,    /* 1 - Back Bottom	Right*/
						0.058, 0.008, 0.066,    /* 2 - Back Top		Right*/
						0.000, 0.008, 0.066,    /* 3 - Back Top		Left */
						0.000, 0.000, 0.001,    /* 4 - Front Bottom	Left */
						0.058, 0.000, 0.001,    /* 5 - Front Bottom	Right*/
						0.066, 0.000, 0.066,    /* 6 - Front Top		Right*/
						0.000, 0.000, 0.066 };  /* 7 - Front Top		Left*/

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

void mazeWall(int v1, int v2, int v3, int v4, int color) {
	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + color * 3);

	glNormal3fv(normals + v1 * 3);
	glVertex3fv(vertices + v1 * 3);
	glNormal3fv(normals + v2 * 3);
	glVertex3fv(vertices + v2 * 3);
	glNormal3fv(normals + v3 * 3);
	glVertex3fv(vertices + v3 * 3);
	glNormal3fv(normals + v4 * 3);
	glVertex3fv(vertices + v4 * 3);
	glEnd();
}

void drawWall() {
	mazeWall(0, 3, 2, 1, 0);	mazeWall(2, 3, 7, 6, 1);
	mazeWall(0, 4, 7, 3, 2);	mazeWall(1, 2, 6, 5, 3);
	mazeWall(4, 5, 6, 7, 4);	mazeWall(0, 1, 5, 4, 5);

}

void drawMan() {
	GLfloat	increment;
	increment = sqBorder + sqSize + sqBorder;
	GLfloat positionX = increment * GridPosX - (increment / 2);
	GLfloat positionY = increment * GridPosY - (increment / 2);
	//ManPosX = positionX;
	//ManPosY = positionY;

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + 5 * 3);

	//position Head
	glTranslatef(positionX, positionY, sqSize * .76);
	//glutSolidTorus(0.004, 0.01, 20, 50);  //doughnut
	glutSolidSphere(.009, 17, 17);
	glTranslatef(-positionX, -positionY, -sqSize * .76);

	// torso
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + 4 * 3);
	glTranslatef(positionX, positionY, sqSize * .5);
	glutSolidCube(.014);
	glTranslatef(-positionX, -positionY, -sqSize * .5);

	// leg1
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + 3 * 3);
	glTranslatef(positionX - .003, positionY, sqSize * .35);
	glutSolidCylinder(.005, -.015, 17, 17);
	glTranslatef(-(positionX - .003), -positionY, -sqSize * .35);

	// leg2
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + 3 * 3);
	glTranslatef(positionX + .003, positionY, sqSize * .35);
	glutSolidCylinder(.005, -.015, 17, 17);
	glTranslatef(-(positionX + .003), -positionY, -sqSize * .35);

	glPopMatrix();
	updateManLocation(positionX, positionY);
}

void drawBorder(int x, int y, int viewportWidth, int viewportHeight) {
	glViewport((GLsizei) x, 
		(GLsizei) y, 
		(GLint)viewportWidth, (GLint)viewportHeight); 
	glColor3f(1.0, 0.0, 0.0);       // for the Red - need multiple windows

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINE_STRIP);
	glVertex3f(.5f, 0.5f, 0.0f);  //upper right
	glVertex3f(-.5f, 0.50f, 0.0f); //upper left
	glVertex3f(-.5f, -0.50f, 0.0f);  // lower left
	glVertex3f(.5f, -0.5f, 0.0f); //Lower right
	glVertex3f(.5f, 0.5f, 0.0f); //upper right
	glEnd();
	glPopMatrix(); glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

void mazeFloor(GLfloat ManPosX, GLfloat ManPosY, GLfloat ViewAngleX, GLfloat ViewAngleY, GLfloat ViewAngleZ) {
	char X_Label[] = "X-Axis";
	char Y_Label[] = "Y-Axis";
	char Z_Label[] = "Z-Axis";

	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(0.50, 0.5, 0.0);

	//glEnable(GL_LINES);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(.4f, 0.4f, 0.0f);
	glVertex3f(0.6f, 0.4f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();

	//glEnable(GL_LINES);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.4f, 0.6f, 0.0f);
	glVertex3f(0.6f, 0.6f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();
	//glLoadIdentity();
	glPopMatrix();
	glTranslatef(0.50, 0.5, 0.0);
	
	glLoadIdentity();
	//glTranslatef(-ManPosX, -ManPosY, 0);
	glRotatef(ViewAngleX, 1.0f, 0.0f, 0.0f);
	glRotatef(ViewAngleY, 0.0f, 1.0f, 0.0f);
	glRotatef(ViewAngleZ, 0.0f, 0.0f, 1.0f);
	glTranslatef(-ManPosX,-ManPosY, 0);

	drawMan();

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
				glVertex3f(xlr, ylr, 0);  // Lower right
				glVertex3f(xur, yur, 0);  // upper right
				glEnd();
				glPushMatrix();
				glTranslatef(xlr, ylr, 0);
				glRotatef(90.0f, 0.0, 0.0, 1.0);
				drawWall();
				glPopMatrix();
			}
			if (squares[blockNumber] & 2) {
				glBegin(GL_LINES);
				glVertex3f(xur, yur, 0);  // upper right
				glVertex3f(xul, yul, 0);  // upper left
				glEnd();
				glPushMatrix();
				glTranslatef(xur, yur, 0);
				glRotatef(180.0f, 0.0, 0.0, 1.0);
				drawWall();
				glPopMatrix();
			}
			if (squares[blockNumber] & 4) {
				glBegin(GL_LINES);
				glVertex3f(xul, yul, 0);  // upper left
				glVertex3f(xll, yll, 0);  // lower left
				glEnd();
				glPushMatrix();
				glTranslatef(xul, yul, 0);
				glRotatef(270.0f, 0.0, 0.0, 1.0);
				drawWall();
				glPopMatrix();
				//printf("xll=%.2f\n", xll);
			}
			if (squares[blockNumber] & 8) {
				glBegin(GL_LINES);
				glVertex3f(xll, yll, 0);  // lower left
				glVertex3f(xlr, ylr, 0);  // Lower right
				glEnd();
				glPushMatrix();
				glTranslatef(xll, yll, 0);
				//glRotatef(0.0f, 0.0, 0.0, 1.0);
				drawWall();
				glPopMatrix();
			}
		}
	}
}
void moveToGridPos(int x, int y) {
	GLfloat	increment;
	increment = sqBorder + sqSize + sqBorder;
	GLfloat positionX = increment * x - (increment / 2);
	GLfloat positionY = increment * y - (increment / 2);

	GridPosX = x;
	GridPosY = y;
	updateManLocation(positionX, positionY);
}