#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)

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
int bWallTextureOn = 1;
int bShieldsUp = 1;

GLint TexImgWidth, TexImgHeight, Texmax;
FILE* infile;
char dummy[80];
GLubyte* TexBits; /* Texture pixmap readfile interface */
//GLubyte* TexBitsWall; /* Texture pixmap */
//GLubyte* TexBitsFloor; /* Texture pixmap */

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
GLfloat floorVertices[] = { -1.0, -1.0, 0.0,
						2.0, -1.0, 0.0,    
						2.0, 2.0, 0.0,    
						-1.0, 2.0, 0.0 };  
const int cRed = 0, cPurple = 1, cGreen = 2, cBlue = 3, cYellow = 4, cWhite = 5;
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

GLfloat  specular_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat  specular_property[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLint	 shininess1 = 40;

// Setting for GL_LIGHT1
// Green Spotlight with narrow angle
GLfloat diffuse_light1[] = { 0.2, 0.4, 0.9, 1.0 };
GLfloat light_position1[] = { 0.0, 0.0, 0.05, 1.0 };
GLfloat spot_direction1[] = { 0.0, 1.0, 0.0 };
GLint	 exponent = 0;
GLfloat	 cutoff = 10.0;

GLfloat fogColor[4] = { .10f,.10f,.10f,1.0f };
GLfloat FogDensity= 5.0;
GLfloat FogVisability = .5;
int FogOn = -1;           // >0 = on  <0 = off

void SetWallTextureOn() {
	bWallTextureOn = 1;
}

void SetWallTextureOff() {
	bWallTextureOn = 0;
}

void TurnOnShield() {
	bShieldsUp = 1;
}

void TurnOffShield() {
	bShieldsUp = 0;
}

void IncreaseCutoffAngle(){
	cutoff += 1;
	if (cutoff >= 45) cutoff = 45;
}

void DecreaseCutoffAngle(){
	cutoff -= 1;
	if (cutoff <= 0) cutoff = 1;
}

void IncreaseFogDensity() {
	FogVisability += .1;
	if (FogVisability >1 ) FogVisability = 1;
	//glFogf(GL_FOG_DENSITY, FogDensity);
	glFogf(GL_FOG_END, FogVisability); // Fog End Depth
	glEnable(GL_FOG);
}

void DecreaseFogDensity() {
	FogVisability -= .1;
	if (FogVisability <= 0.1) FogVisability = 0.1;
	//glFogf(GL_FOG_DENSITY, FogDensity);
	glFogf(GL_FOG_END, FogVisability); // Fog End Depth
	glEnable(GL_FOG);
}

void mazeWall(int v1, int v2, int v3, int v4, int color) {
	int range = 1;   //  effectivly - a zoom for the texture
	glBegin(GL_POLYGON);
	//glBegin(GLU_LINE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + color * 3);


	glTexCoord2f(0.0, 0.0);
	glNormal3fv(normals + v1 * 3);
	glVertex3fv(vertices + v1 * 3);

	glTexCoord2f(0.0, range);
	glNormal3fv(normals + v2 * 3);
	glVertex3fv(vertices + v2 * 3);

	glTexCoord2f(range, range);
	glNormal3fv(normals + v3 * 3);
	glVertex3fv(vertices + v3 * 3);

	glTexCoord2f(range, 0.0);
	glNormal3fv(normals + v4 * 3);
	glVertex3fv(vertices + v4 * 3);
	glEnd();
}

void DrawFloor() {
	int range = 1;



	read_file("BigPebble.ppm");
	//TexBitsFloor = TexBits;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TexImgWidth, TexImgHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, TexBits);
	if (bWallTextureOn) {
		glEnable(GL_TEXTURE_2D);
	}
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();

	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + cPurple * 3);

	glTexCoord2f(0.0, 0.0); 
	glNormal3fv(normals + 0 * 3);
	glVertex3fv(floorVertices + 0 * 3);

	glTexCoord2f(0.0, range); 
	glNormal3fv(normals + 3 * 3);
	glVertex3fv(floorVertices + 1 * 3);

	glTexCoord2f(range, range);
	glNormal3fv(normals + 2 * 3);
	glVertex3fv(floorVertices + 2 * 3);

	glTexCoord2f(range, 0.0); 
	glNormal3fv(normals + 1 * 3);
	glVertex3fv(floorVertices + 3 * 3);
	glEnd();
	glPopMatrix();

	read_file("SmallTile90.ppm");
	//TexBitsWall = TexBits;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TexImgWidth, TexImgHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, TexBits);

}

void drawWall() {
	mazeWall(0, 3, 2, 1, 0);	mazeWall(2, 3, 7, 6, 1);
	mazeWall(0, 4, 7, 3, 2);	mazeWall(1, 2, 6, 5, 3);
	mazeWall(4, 5, 6, 7, 4);	mazeWall(0, 1, 5, 4, 5);

}

void drawMan(GLfloat ViewAngleZ) {
	GLfloat	increment;
	increment = sqBorder + sqSize + sqBorder;
	GLfloat positionX = increment * GridPosX - (increment / 2);
	GLfloat positionY = increment * GridPosY - (increment / 2);
	//ManPosX = positionX;
	//ManPosY = positionY;
	set_up_light();
	DrawFloor();

	glPushMatrix();
	glTranslatef(positionX, positionY, sqSize * .76);
	glRotatef(-ViewAngleZ, 0.0f, 0.0f, 1.0f);
	light_position1[0] = positionX;
	light_position1[1] = positionY-.1;
	light_position1[2] = 0;
	light_position1[3] = 1;

	spot_direction1[0] = sin(-ViewAngleZ);
	spot_direction1[1] = cos(-ViewAngleZ);
	spot_direction1[2] = 0;
	glEnable(GL_LIGHT1);
	//glRotatef(ViewAngleZ, 0.0f, 0.0f, 1.0f);
	glTranslatef(-positionX, -positionY, -sqSize * .76);
	glPushMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + cWhite * 3);

	//position Head
	glTranslatef(positionX, positionY, sqSize * .76);
	//glutSolidTorus(0.004, 0.01, 20, 50);  //doughnut
	glutSolidSphere(.009, 17, 17);
	glTranslatef(-positionX, -positionY, -sqSize * .76);

	// torso
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + cYellow * 3);
	glTranslatef(positionX, positionY, sqSize * .5);
	glutSolidCube(0.014);
	glTranslatef(-positionX, -positionY, -sqSize * .5);

	// leg1
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + cBlue * 3);
	glTranslatef(positionX - .003, positionY, sqSize * .35);
	glutSolidCylinder(.005, -.015, 17, 17);
	glTranslatef(-(positionX - .003), -positionY, -sqSize * .35);

	// leg2
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + cBlue * 3);
	glTranslatef(positionX + .003, positionY, sqSize * .35);
	glutSolidCylinder(.005, -.015, 17, 17);
	glTranslatef(-(positionX + .003), -positionY, -sqSize * .35);

	// Arm RIGHT UPPER
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + cYellow * 3);
	glTranslatef(positionX + .004, positionY, sqSize * .55);
	glRotatef(165, 0, 1, 0);
	glRotatef(-5, 1, 0, 0);
	glutSolidCylinder(.004, .015, 17, 17);
	glRotatef(5, 1, 0, 0);
	glRotatef(-165, 0, 1, 0);
	glTranslatef(-(positionX + .004), -positionY, -sqSize * .55);

	// Arm Left Upper
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + cYellow * 3);
	glTranslatef(positionX - .004, positionY, sqSize * .55);
	glRotatef(-165, 0, 1, 0);
	glutSolidCylinder(.004, .015, 17, 17);
	glRotatef(165, 0, 1, 0);
	glTranslatef(-(positionX - .004), -positionY, -sqSize * .55);

	// Arm Right Lower
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + cWhite * 3);
	glTranslatef(positionX + .008, positionY+.003, sqSize * .32);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(.0035, .006, 17, 17);
	glRotatef(90, 1, 0, 0);
	glTranslatef(-(positionX + .008), -(positionY+.003), -sqSize * .32);

	// Arm Left Lower
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + cWhite * 3);
	glTranslatef(positionX - .008, positionY, sqSize * .32);
	glRotatef(180, 1, 0, 0);
	glutSolidCylinder(.0035, .005, 17, 17);
	glRotatef(-180, 1, 0, 0);
	glTranslatef(-(positionX - .008), -positionY, -sqSize * .32);

	glPopMatrix();
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
	/*
	glBegin(GL_LINE_STRIP);
	glVertex3f(.5f, 0.5f, 0.0f);  //upper right
	glVertex3f(-.5f, 0.50f, 0.0f); //upper left
	glVertex3f(-.5f, -0.50f, 0.0f);  // lower left
	glVertex3f(.5f, -0.5f, 0.0f); //Lower right
	glVertex3f(.5f, 0.5f, 0.0f); //upper right
	glEnd();
	*/
	glPopMatrix(); glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

void mazeFloor(GLfloat ManPosX, GLfloat ManPosY, GLfloat ViewAngleX, GLfloat ViewAngleY, GLfloat ViewAngleZ) {
	char X_Label[] = "X-Axis";
	char Y_Label[] = "Y-Axis";
	char Z_Label[] = "Z-Axis";

	GLfloat	increment;
	increment = sqBorder + sqSize + sqBorder;

	GLfloat positionX = increment * GridPosX - (increment / 2);
	GLfloat positionY = increment * GridPosY - (increment / 2);

	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(0.50, 0.5, 0.0);
/*
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
	*/
	//glLoadIdentity();
	glPopMatrix();
	glTranslatef(0.50, 0.5, 0.0);
	
	glLoadIdentity();

	//glTranslatef(-ManPosX, -ManPosY, 0);
	glRotatef(ViewAngleX, 1.0f, 0.0f, 0.0f);
	glRotatef(ViewAngleY, 0.0f, 1.0f, 0.0f);
	glRotatef(ViewAngleZ, 0.0f, 0.0f, 1.0f);
	glTranslatef(-ManPosX,-ManPosY, 0);

	glDisable(GL_TEXTURE_2D);
	
	if (bShieldsUp) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
			colors + cRed * 3);

		//position Shield
		glTranslatef(positionX, positionY, sqSize * .76);
		//glutSolidTorus(0.004, 0.01, 20, 50);  //doughnut
		glutSolidSphere(.1, 17, 17);
		glTranslatef(-positionX, -positionY, -sqSize * .76);
	}

	drawMan(ViewAngleZ);
	if (bWallTextureOn) {
		glEnable(GL_TEXTURE_2D);
	}
	else {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);
		//glEnable(GL_POLYGON_OFFSET_FILL);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
	}

	GLfloat xll, xlr, xul, xur;
	GLfloat yll, ylr, yul, yur;
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
				//glLoadIdentity();
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

	//blockNumber = (y - 1) * mazeWidth + (x - 1);
	blockNumber = (GridPosY - 1) * mazeWidth + (GridPosX - 1);
	if (x>=0 && x<=16 && y>=0 && y<=11) {
	if (squares[blockNumber] & 1) {
		printf("Can not go East. %d,%d,%d\t\t", GridPosX, GridPosY, squares[blockNumber]);
		if (x > GridPosX) x = GridPosX;
	}
	if (squares[blockNumber] & 2) {
		printf("Can not go North. %d,%d,%d\t\t", GridPosX, GridPosY, squares[blockNumber]);
		if (y > GridPosY) y = GridPosY;
	}
	if (squares[blockNumber] & 4) {
		printf("Can not go West. %d,%d,%d\t\t", GridPosX, GridPosY, squares[blockNumber]);
		if (x < GridPosX) x = GridPosX;
	}
	if (squares[blockNumber] & 8) {
		printf("Can not go South. %d,%d,%d\t\t", GridPosX, GridPosY, squares[blockNumber]);
		if (y < GridPosY) y = GridPosY;
	}
	printf("\n");

}
	SetManGridValues(x, y);
	GLfloat positionX = increment * x - (increment / 2);
	GLfloat positionY = increment * y - (increment / 2);


	GridPosX = x;
	GridPosY = y;
	updateManLocation(positionX, positionY);
	blockNumber = (GridPosY - 1) * mazeWidth + (GridPosX - 1);
	/*
	if (squares[blockNumber] & 1) {
		printf("Can not go East. %d,%d,%d\t\t", GridPosX, GridPosY, squares[blockNumber]);
	}
	if (squares[blockNumber] & 2) {
		printf("Can not go North. %d,%d,%d\t\t", GridPosX, GridPosY, squares[blockNumber]);
	}
	if (squares[blockNumber] & 4) {
		printf("Can not go West. %d,%d,%d\t\t", GridPosX, GridPosY, squares[blockNumber]);
	}
	if (squares[blockNumber] & 8) {
		printf("Can not go South. %d,%d,%d\t\t", GridPosX, GridPosY, squares[blockNumber]);
	}
	printf("\n");
	*/
}

void set_up_light(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	// Spot Light 1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);

	//glLoadIdentity();
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	//glLoadIdentity();
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
	glLighti(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
	glEnable(GL_LIGHT1);

	// Set Material properties to follow glColor values
	glMateriali(GL_FRONT, GL_SHININESS, shininess1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

void read_file(char* argv)
{
	int i, temp;
	GLubyte* sp, c;

	if ((infile = fopen(argv, "rb+")) == NULL)
	{
		printf("File open error\n");
		exit(1);
	}
	do { temp = fgetc(infile); } while (temp != '\n');
	do { temp = fgetc(infile); } while (temp != '\n');		fscanf(infile, "%s", dummy);	TexImgWidth = atoi(dummy);
	fscanf(infile, "%c", &c);
	fscanf(infile, "%s", dummy);	TexImgHeight = atoi(dummy);
	fscanf(infile, "%c", &c);
	fscanf(infile, "%s", dummy);	Texmax = atoi(dummy);
	fscanf(infile, "%c", &c);									// return after max

	if (TexBits != 0) free(TexBits);
	TexBits = (GLubyte*)calloc(TexImgWidth * TexImgHeight * 3, sizeof(GLubyte));
	for (i = TexImgHeight - 1; i >= 0; i--)
	{
		sp = TexBits + i * TexImgWidth * 3;
		fread(sp, sizeof(GLubyte), TexImgWidth * 3, infile);
	}
	fclose(infile);
}

void FogToggle() {
	FogOn = -FogOn;
	if (FogOn > 0) glEnable(GL_FOG);
	else glDisable(GL_FOG);
}

void MazeInit() {
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, FogDensity);
	glHint(GL_FOG_HINT, GL_DONT_CARE); glFogf(GL_FOG_START, 0.0150f); // Fog Start Depth
	glFogf(GL_FOG_END, FogVisability); // Fog End Depth
	FogToggle();
}
