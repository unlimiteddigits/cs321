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

const int cX = 0, cY = 1, cZ = 2;

int animationActive = -1;

GLfloat windowWidth = VIEWSTARTW;
GLfloat windowHeight = VIEWSTARTH;
GLfloat viewportWidth = VIEWSTARTW;
GLfloat viewportHeight = VIEWSTARTH;
GLfloat viewportXOffset = 0;
GLfloat viewportYOffset = 0;
int bPerspectiveMode = 0;

GLdouble eye[3] = { -0.5,0.5,2.0 };
//GLdouble eye[3] = { 0.5,-0.5, -0.0 }; // Starting here doesn't
GLdouble center[3] = { -0.5,0.50,0 };
GLdouble up[3] = { 0,1,0 };

double ViewAngleX = 0;
double ViewAngleY = 0;
double ViewAngleZ = 0;
double AtAngleX = 0;
double AtAngleY = 0;
double AtAngleZ = 0;

static GLint width, height;
GLfloat orthoLeft = ORTHOLEFTSTART;
GLfloat orthoRight = ORTHORIGHTSTART;
GLfloat orthoBottom = ORTHOBOTTOMSTART; 
GLfloat orthoTop = ORTHOTOPSTART;
GLfloat orthoNear = ORTHONEARSTART; 
GLfloat orthoFar = ORTHOFARSTART;
GLfloat moveStep = 0.01;


GLfloat emission_light[] = { 0.5, 0.5, 1.0, 1.0 };

GLfloat light_directional[] = { -2.0, 1.0, 2.0, 0.0 };  //shader 8 includes angle

// Setting for GL_LIGHT0
GLfloat  ambient_light[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat  diffuse_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat  light_position[] = { 0.0, 0.0, 5.0, 1.0 };
GLint	 shininess = 40;


// Setting for GL_LIGHT2
// Red Spotlight with wide angle
GLfloat diffuse_light2[] = { 1.0, 0.2, 0.6, 1.0 };
GLfloat light_position2[] = { 0.3, -0.5, 4.0, 1.0 };
GLfloat spot_direction2[] = { 0.3, -0.5, -2.0 };


int bTopView = -1;
GLfloat ManPosX = 0, ManPosY = 0;
int ManGridX=11, ManGridY = 0;

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize((int)windowWidth, (int)windowHeight);
	glutInitWindowPosition((int)(glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2, (int)(glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2);  // auto version -> 
	glutCreateWindow("Maze");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void other_init()
{
	glutCreateMenu(menu);
	glutAddMenuEntry("Motion", 1);
	glutAddMenuEntry("WireFrame", 2);
	glutAddMenuEntry("Ortho Mode", 3);
	glutAddMenuEntry("Perspective Mode", 4);
	glutAddMenuEntry("Top View", 5);
	glutAddMenuEntry("Wall Texture Off", 6);
	glutAddMenuEntry("Wall Texture On", 7);
	glutAddMenuEntry("Shield Off", 8);
	glutAddMenuEntry("Shield On - hidden from inside", 9);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutIdleFunc(DoBackgroundStuff);    // playing with more functions
	glClearColor(0.0, 0.0, 0.50, 1.0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,	diffuse_light);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);         // Get Back to the Modelview
	moveToGridPos(ManGridX, ManGridY);
	glPolygonMode(GL_FRONT, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}

void DoBackgroundStuff() {

	GLfloat radians = 0;

	GLfloat	increment;
	increment = .066;
	GLfloat positionX = increment * 11 - (increment / 2);
	GLfloat positionY = -(increment / 2);
//	ManPosX = positionX;
//	ManPosY = positionY;

	if (bTopView==-1)     // 
	{
		//printf("Doing idle Stuff...\n"); 
		bTopView = 0;
		for (double i = 0; i >= -90.0; i=i-5) {
			ViewAngleX = i;
			radians = (GLfloat)(ViewAngleX * (GLfloat)(M_PI / 180));
			eye[cZ] = (GLfloat)(0.5) * cos(radians);
			eye[cY] = (GLfloat)(0.5) * sin(radians);
			if (eye[cZ] < 0)
				up[cY] = -1.0;
			if (eye[cZ] >= 0)
				up[cY] = 1.0;
			display();
		}
		 
		eye[cX] = ManPosX;
		eye[cY] = ManPosY-.1;
		eye[cZ] = .366;
		center[cX] = ManPosX;
		center[cY] = ManPosY;
		center[cZ] = .366;
		bPerspectiveMode = 1;
		reshape(viewportWidth, viewportHeight);
		
		//viewportWidth
	}

}

void change_view()
{
	//glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);

	gluLookAt(eye[cX], eye[cY], eye[cZ], center[cX], center[cY], center[cZ], up[cX], up[cY], up[cZ]);
}

void drawString(double x, double y, double z, char* mystring) {
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);

	for (char* c = mystring; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);  // Updates the position
	}
}

void DrawAxisLines()
{
	char X_Label[] = "X-Axis";
	char Y_Label[] = "Y-Axis";
	char Z_Label[] = "Z-Axis";

	//glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();

	//sprintf("", Axis_Label, "X-Axis");
	drawString(1.5, .1, 0, X_Label);

	//Axis_Label = "Y-Axis";
	drawString(.1, 1.5, 0, Y_Label);

	//Axis_Label = "Z-Axis";
	drawString(.1, .100, 1.5, Z_Label);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0);
	glEnd();
	//glDisable(GL_LINE_STIPPLE);
}

void display(void)
{
	char sResult[200];
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	change_view();
	drawBorder(0, 0, 600, 400);


	mazeFloor(ManPosX, ManPosY, ViewAngleX, ViewAngleY, ViewAngleZ);
	//glLoadIdentity();
	glPushMatrix();

	 //DrawAxisLines();
	//glTranslatef(-1.2, 1.2, 0.0);
	//glutSolidTeapot(0.8);
	//glTranslatef(2.0, -2.0, 0.0);
	//glutSolidSphere(0.8, 20, 16);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
	
	/* draw triangulation lines 
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(eye[cX], -1.0, -0.032f);  //upper right
	glVertex3f(0, 0, -0.032f); //upper left
	glVertex3f(center[cX], center[cY], -0.010); //upper left
	glVertex3f(eye[cX], eye[cY], -0.032f);  //upper right
	glEnd();
	*/

	//Status info
	//glLoadIdentity();
	sprintf_s(sResult, "eyeX=%0.3f eyeY=%0.3f eyeZ=%0.3f \nAtx=%0.3f Aty=%0.3f AtZ=%0.3f \nupX=%0.1f upY=%0.1f upZ=%0.1f", (float)eye[cX], (float)eye[cY], (float)eye[cZ], (float)center[cX], (float)center[cY], (float)center[cZ], (float)up[cX], (float)up[cY], (float)up[cZ]);
	drawString(-.50, 1.0, 0, sResult);
	sprintf_s(sResult, "Light Position: X=%0.1f Y=%0.1f Z=%0.1f (Numeric Keypad 2-8)  Plus u,v,w,r,g,b,d,f,p,i", (float)light_position[cX], (float)light_position[cY], (float)light_position[cZ]);
	drawString(-.5, 0.85, 0, sResult);

	//glViewport((GLsizei)0, (GLsizei)(windowHeight - viewportHeight), (GLint)viewportWidth, (GLint)viewportHeight);
	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (bPerspectiveMode) {
		//glFrustum(orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar);
		//glPushMatrix();
		glTranslatef(0.0f, 0.052f, 0.015f);
		gluPerspective(105, 1.0f, .005, 5.7);
		glTranslatef(0.0f, -0.052f, -0.015f);
		//glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		glViewport(w / 2, h / 2, w / 2, h / 2);
		//gluLookAt(-2500.0f, 2500.0f, 2500.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	}
	else {
		glViewport(0, 0, w / 3, h);
		if (w >= h)
			glOrtho(orthoLeft, orthoRight,
				orthoBottom * (GLfloat)h / (GLfloat)w, orthoTop * (GLfloat)h / (GLfloat)w,
				orthoNear, orthoFar * 2);
		else
			glOrtho(orthoLeft * (GLfloat)w / (GLfloat)h, orthoRight * (GLfloat)w / (GLfloat)h,
				orthoBottom, orthoTop, orthoNear, orthoFar * 2);
	}
	//glOrtho(-.010 * (GLfloat)w / (GLfloat)h, 1.01 * (GLfloat)w / (GLfloat)h, -0.01, 1.01, 1, -1);
	//glOrtho(0, 1, 0, 1, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	printf("oL=%.3f oR=%.3f oB=%.3f oT=%.3f oN=%.3f oF=%.3f w=%d h=%d\n", orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar, w, h);
	printf("eX=%.3f eY=%.3f eZ=%.3f cX=%.3f cY=%.3f cZ=%.3f uX=%.1f uY=%.1f uZ=%.1f\n", eye[cX], eye[cY], eye[cZ], center[cX], center[cY], center[cZ], up[cX], up[cZ]);
}


int count = 0;

void
animation(void)
{
	/* animate the cone */
	GLfloat Prev_X = ViewAngleX;
	GLfloat Prev_Y = ViewAngleY;
	GLfloat Prev_Z = ViewAngleZ;

	ViewAngleX += 2.0;
	ViewAngleY -= 1.0;
	ViewAngleZ += 2.0;
	if (ViewAngleX >= 360)
		ViewAngleX = 0.0;
	if (ViewAngleY <= -360)
		ViewAngleY = 0.0;
	if (ViewAngleZ >= 360)
		ViewAngleZ = 0.0;
	glutPostRedisplay();
	count++;
	if (count >= 600) {
		ViewAngleX =Prev_X;
		ViewAngleY = Prev_Y;
		ViewAngleZ = Prev_Z;
		glutPostRedisplay();
		glutIdleFunc(NULL);
	}
}

void AnimateOverhead() {

	if (ViewAngleY ==1.0) {
		if (ViewAngleX < -85.0) {
				ViewAngleX +=1;
			}
		ViewAngleZ += 1.0;
		if (ViewAngleZ >= 360)
			ViewAngleZ = 0.0;
		glutPostRedisplay();
	}
}

void
menu(int choice)
{
	switch (choice) {
	case 1:
		count = 0;
		glutIdleFunc(animation);
		break;
	case 2:
		glutSetWindowTitle("WireFrame");
		glPolygonMode(GL_FRONT, GL_LINE);
		SetWallTextureOff();
		glutPostRedisplay();
		break;
	case 3:
		SetWallTextureOn();
		glPolygonMode(GL_FRONT, GL_FILL);
		glutSetWindowTitle("Ortho Mode");
		SetOrthoGroundView();
		glutPostRedisplay();
		break;
	case 4:
		SetWallTextureOn();
		glPolygonMode(GL_FRONT, GL_FILL);
		SetOrthoGroundView();
		bTopView = 0;
		bPerspectiveMode = 1;
		reshape(viewportWidth, viewportHeight);
		glutSetWindowTitle("Perspective Mode");
		glutPostRedisplay();
		break;
	case 5:
		EnableTopView();
		glutPostRedisplay();
		break;
	case 6:
		SetWallTextureOff();
		display();
		break;
	case 7:
		SetWallTextureOn();
		display();
		break;
	case 8:
		TurnOffShield();
		display();
		break;
	case 9:
		TurnOnShield();
		display();
		break;
	}
}

void specialKeyboardKeys(int key, int x, int y) {
	printf("Speacial Key Pressed = %d  => ", key);
	float xMove,yMove;
	GLfloat radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));;

	switch (key)
	{
	case GLUT_KEY_UP:
		xMove = sin(radians);
		yMove = cos(radians);
		if (yMove < .5 && yMove >= 0) yMove = 0;
		if (xMove < .5 && xMove >= 0) xMove = 0;
		if (yMove > -.5 && yMove < 0) yMove = 0;
		if (xMove > -.5 && xMove < 0) xMove = 0;
		if (yMove > .5) yMove = 1;
		if (xMove > .5) xMove = 1;
		if (yMove < -.5) yMove = -1;
		if (xMove < -.5) xMove = -1;
		printf("Arrow Up\n");
		ManGridX += xMove;
		ManGridY += yMove;
		moveToGridPos(ManGridX, ManGridY);
		eye[cX] = ManPosX;
		eye[cY] = ManPosY ;
		eye[cZ] = 1;
		center[cX] = ManPosX;
		center[cY] = ManPosY+.1;
		up[cY] = 1;
		//ViewAngleZ = 0;
		break;
	case GLUT_KEY_DOWN:
		xMove = sin(radians);
		yMove = cos(radians);
		if (yMove < .5 && yMove >= 0) yMove = 0;
		if (xMove < .5 && xMove >= 0) xMove = 0;
		if (yMove > -.5 && yMove < 0) yMove = 0;
		if (xMove > -.5 && xMove < 0) xMove = 0;
		if (yMove > .5) yMove = 1;
		if (xMove > .5) xMove = 1;
		if (yMove < -.5) yMove = -1;
		if (xMove < -.5) xMove = -1;
		printf("Arrow Down\n");
		ManGridX -= xMove;
		ManGridY -= yMove;
		moveToGridPos(ManGridX, ManGridY);
		eye[cX] = ManPosX;
		eye[cY] = ManPosY;
		eye[cZ] = 1;
		center[cX] = ManPosX;
		center[cY] = ManPosY-.1;
		up[cY] = 1;
		//if (bTopView!=1) ViewAngleZ = 180;
		break;
	case GLUT_KEY_LEFT:
		ViewAngleZ -= 15;
		printf("Arrow Left %.1f\n", ViewAngleZ);
		break;
		ManGridX -= 1;
		moveToGridPos(ManGridX, ManGridY);
		eye[cX] = ManPosX;
		eye[cY] = ManPosY;
		eye[cZ] = 1;
		center[cX] = ManPosX - .1;
		center[cY] = ManPosY;
		//if (bTopView != 1) ViewAngleZ = -90;
		break;
	case GLUT_KEY_RIGHT:
		ViewAngleZ += 15;
		printf("Arrow Right %.1f\n",ViewAngleZ);
		break;
		ManGridX += 1;
		moveToGridPos(ManGridX, ManGridY);
		eye[cX] = ManPosX;
		eye[cY] = ManPosY;
		eye[cZ] = 1;
		center[cX] = ManPosX + .1;
		center[cY] = ManPosY;
		//if (bTopView != 1) ViewAngleZ = 90;
		break;
	case GLUT_KEY_F1:
		printf("F1\n");
		break;
	case GLUT_KEY_F2:
		printf("F2\n");
		break;
	case GLUT_KEY_F3:
		printf("F3\n");
		break;
	case GLUT_KEY_F4:
		printf("F4\n");
		break;
	case GLUT_KEY_F5:
		printf("F5\n");
		break;
	case GLUT_KEY_F6:
		printf("F6\n");
		break;
	case GLUT_KEY_F7:
		printf("F7\n");
		break;
	case GLUT_KEY_F8:
		printf("F8\n");
		break;
	case GLUT_KEY_F9:
		printf("F9\n");
		break;
	case GLUT_KEY_F10:
		printf("F10\n");
		break;
	case GLUT_KEY_F11:
		printf("F11\n");
		break;
	case GLUT_KEY_F12:
		printf("F12\n");
		break;
	case GLUT_KEY_PAGE_UP:
		printf("Page up\n");
		break;
	case GLUT_KEY_PAGE_DOWN:
		printf("Page Down\n");
		break;
	case GLUT_KEY_HOME:
		printf("Home\n");
		break;
	case GLUT_KEY_END:
		printf("End\n");
		break;
	case GLUT_KEY_INSERT:
		printf("Insert\n");
		break;
	}

	display();
}

void keyboard(unsigned char key, int x, int y)
{
	GLfloat radians = 0;
	int ii = 0;

	//printf("Key Pressed = %c\n", key);
	switch (key)
	{
	case'A':
		for (ii = 0; ii < 3; ii++) {
			ambient_light[ii] += .1;
			if (ambient_light[ii] >= 1.0) ambient_light[ii] = 1.0;
		}
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		glEnable(GL_LIGHT0);
		break;
	case 'a':
		for (ii = 0; ii < 3; ii++) {
			ambient_light[ii] -= .1;
			if (ambient_light[ii] <= 0) ambient_light[ii] = 0;
		}
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		glEnable(GL_LIGHT0);
		break;
	case'D':
		for (ii = 0; ii < 3; ii++) {
			diffuse_light[ii] += .1;
			if (diffuse_light[ii] >= 1.0) diffuse_light[ii] = 1.0;
		}
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, diffuse_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, diffuse_light);
		glEnable(GL_LIGHT0);
		break;
	case 'd':
		for (ii = 0; ii < 3; ii++) {
			diffuse_light[ii] -= .1;
			if (diffuse_light[ii] <= 0) diffuse_light[ii] = 0;
		}
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, diffuse_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, diffuse_light);
		glEnable(GL_LIGHT0);
		break;
	case'E':
		for (ii = 0; ii < 3; ii++) {
			emission_light[ii] += .1;
			if (emission_light[ii] >= 1.0) emission_light[ii] = 1.0;
		}
		glLightfv(GL_LIGHT0, GL_EMISSION, emission_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		glEnable(GL_LIGHT0);
		break;
	case 'e':
		for (ii = 0; ii < 3; ii++) {
			emission_light[ii] -= .1;
			if (emission_light[ii] <= 0) emission_light[ii] = 0;
		}
		glLightfv(GL_LIGHT0, GL_EMISSION, emission_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		glEnable(GL_LIGHT0);
		break;
	case 'X': ViewAngleX += 5;
		radians = (GLfloat)(ViewAngleX * (GLfloat)(M_PI / 180));
		eye[cZ] = 1+(GLfloat)(0.5) * cos(radians);
		eye[cY] = (GLfloat)(0.5) * sin(radians);
		if (eye[cZ] < 0)
			up[cY] = -1.0;
		if (eye[cZ] >= 0)
			up[cY] = 1.0;
		break;
	case 'x': ViewAngleX -= 5;
		radians = (GLfloat)(ViewAngleX * (GLfloat)(M_PI / 180));
		eye[cZ] = 1 + (GLfloat)(.5) * cos(radians);
		eye[cY] = (GLfloat)(.5) * sin(radians);
		if (eye[cZ] < 0)
			up[cY] = (GLfloat)-1.0;
		if (eye[cZ] >= 0)
			up[cY] = (GLfloat)1.0;
		break;
	case 'Y':
		/*
		//glPushMatrix();
		glTranslatef(-ManPosX, -ManPosY, -.066);
		glRotatef(5.0, 0, 1, 0);
		glTranslatef(ManPosX, ManPosY, .066);
		//glPopMatrix();
		*/
		ViewAngleY += 5;
		radians = (GLfloat)(ViewAngleY * (GLfloat)(M_PI / 180));
		eye[cZ] = (GLfloat)(0.5) * cos(radians);
		eye[cX] = (GLfloat)(0.5) * sin(radians);
		up[cY] = 1.0;

		break;
	case 'y':
		ViewAngleY -= 5;
		radians = (GLfloat)(ViewAngleY * (GLfloat)(M_PI / 180));
		eye[cZ] = (GLfloat)(0.5) * cos(radians);
		eye[cX] = (GLfloat)(0.5) * sin(radians);
		up[cY] = 1.0;
		break;
	case 'Z': ViewAngleZ += 5;
		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		eye[cX] = (GLfloat)(.5) * sin(radians);
		eye[cY] = (GLfloat)(.5) * cos(radians);
		break;
	case 'z': ViewAngleZ -= 5;
		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		eye[cX] = (GLfloat)(.5) * sin(radians);
		eye[cY] = (GLfloat)(.5) * cos(radians);
		break;

	case 'L': 
		ViewAngleZ += 10.0;
		if (ViewAngleZ <= 0.0) ViewAngleZ = 360.0;

		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		center[cX] = ManPosX+(GLfloat)(.1) * sin(radians);
		center[cY] = ManPosY+ (GLfloat)(.5) * cos(radians);
		eye[cX] = ManPosX; eye[cY] = ManPosY;
		/*
		eye[cX] = 0.7; eye[cY] = -0.6; eye[cZ] = 0.033;
		center[cX] = 0.7; center[cY] = -0.5; center[cZ] = 0.033;
		up[cX] = 0; up[cY] = 0; up[cZ] = 1;
		*/
		printf("eX=%.3f eY=%.3f eZ=%.3f cX=%.3f cY=%.3f cZ=%.3f A=%.1F\n", eye[cX], eye[cY], eye[cZ], center[cX], center[cY], center[cZ], ViewAngleZ);
		break;
	case 'J': 
		ViewAngleZ -= 10.0;
		if (ViewAngleZ >= 360.0) ViewAngleZ = 0;

		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		center[cX] = ManPosX + (GLfloat)(.5) * sin(radians);
		center[cY] = ManPosY + (GLfloat)(.5) * cos(radians);
		eye[cX] = ManPosX; eye[cY] = ManPosY;
		break;
		eye[cX] += moveStep; break; center[cX] += moveStep; break;
	case 'j': eye[cX] -= moveStep; center[cX] -= moveStep; break;
	case 'I': case 'i': eye[cY] += moveStep; center[cY] += moveStep; break;// ManPosY += moveStep; break; //
	case 'K': case 'k': eye[cY] -= moveStep; center[cY] -= moveStep; break;  //ManPosY -= moveStep; break; // 
	case 'U': case 'u': eye[cZ] += moveStep; center[cZ] += moveStep; break;
	case 'V': case 'v': eye[cZ] -= moveStep; center[cZ] -= moveStep; break;
	case '_':
		orthoLeft -= moveStep;
		orthoRight += moveStep;
		orthoBottom -= moveStep;
		orthoTop += moveStep;
		if (orthoRight>10.0) {
			orthoLeft += moveStep;
			orthoRight -= moveStep;
			orthoBottom += moveStep;
			orthoTop -= moveStep;
		}
		reshape(viewportWidth, viewportHeight);
		break;
	case '+':
		orthoLeft += moveStep;
		orthoRight -= moveStep;
		orthoBottom += moveStep;
		orthoTop -= moveStep;
		if (orthoLeft >= orthoRight) {
			orthoLeft -= moveStep;
			orthoRight += moveStep;
			orthoBottom -= moveStep;
			orthoTop += moveStep;
		}
		reshape(viewportWidth, viewportHeight);
		break;
	case 'W': printf("Wireframe mode"); 
		glPolygonMode(GL_FRONT, GL_LINE);
		SetWallTextureOff();
		break;
	case '0':
		SetWallTextureOn();
		moveToGridPos(11, 0);
		ViewAngleX = -90; ViewAngleY = 0; ViewAngleZ = 0;
		SetOrthoGroundView();
		ManGridX = 11;
		ManGridY = 0;
		//bTopView = 1;
		light_position[cX] = -2.0, light_position[cY] = -2.0, light_position[cZ] = 0;
		reshape(viewportWidth, viewportHeight);
		break;
	case 'T': case 't':
		glShadeModel(GL_FLAT);
		break;
	case 'M': case 'm':
		glShadeModel(GL_SMOOTH);
		break;
	case '[':
		IncreaseCutoffAngle();
		break;
	case ']':
		DecreaseCutoffAngle();
		break;
	case '-':
		DecreaseFogDensity();
		break;
	case '=':
		IncreaseFogDensity();
		break;
	case '1':
		light_position[cZ] += 1;
		light_directional[cZ] += 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);	
		printf("light Pos X=%.1f Y=%.1f Z=%.1f\n ", light_position[cX], light_position[cY], light_position[cZ]);
		break;
	case '2':
		light_position[cY] -= 1;
		light_directional[cY] -= 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		printf("light Pos X=%.1f Y=%.1f Z=%.1f\n ", light_position[cX], light_position[cY], light_position[cZ]);
		break;
	case '8':
		light_position[cY] += 1;
		light_directional[cY] += 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		printf("light Pos X=%.1f Y=%.1f Z=%.1f\n ", light_position[cX], light_position[cY], light_position[cZ]);
		break;
	case '4':
		light_position[cX] -= 1;
		light_directional[cX] -= 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_directional);
		break;
	case '6':
		light_position[cX] += 1;
		light_directional[cX] += 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_directional);
		break;
	case '7':
		light_position[cZ] -= 1;
		light_directional[cZ] -= 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glEnable(GL_LIGHT0);
		break;
	case '3':
		light_position[cZ] += 1;
		light_directional[cZ] += 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		break;
	case 'F': 	case 'f':
		FogToggle();
		break;
	case 'R': 
		diffuse_light[0] += .1;
		if (diffuse_light[0] >= 1.0) diffuse_light[0] = 1.0;
		emission_light[0] += .1;
		if (emission_light[0] >= 1.0) emission_light[0] = 1.0;
		ambient_light[0] += .1;
		if (ambient_light[0] >= 1.0) ambient_light[0] = 1.0;
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, diffuse_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, diffuse_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, emission_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		break;
	case 'r':
		diffuse_light[0] -= .1;
		if (diffuse_light[0] <= 0.0) diffuse_light[0] = 0.0;
		emission_light[0] -= .1;
		if (emission_light[0] <= 0.0) emission_light[0] = 0.0;
		ambient_light[0] -= .1;
		if (ambient_light[0] <= 0) ambient_light[0] = 0.0;
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, diffuse_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, diffuse_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, emission_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		break;
	case 'G':
		diffuse_light[1] += .1;
		if (diffuse_light[1] >= 1.0) diffuse_light[1] = 1.0;
		emission_light[1] += .1;
		if (emission_light[1] >= 1.0) emission_light[1] = 1.0;
		ambient_light[1] += .1;
		if (ambient_light[1] >= 1.0) ambient_light[1] = 1.0;
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, diffuse_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, diffuse_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, emission_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		break;
	case 'g':
		diffuse_light[1] -= .1;
		if (diffuse_light[1] <= 0.0) diffuse_light[1] = 0.0;
		emission_light[1] -= .1;
		if (emission_light[1] <= 0.0) emission_light[1] = 0.0;
		ambient_light[1] -= .1;
		if (ambient_light[1] <= 0) ambient_light[1] = 0.0;
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, diffuse_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, diffuse_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, emission_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		break;
	case 'B':
		diffuse_light[2] += .1;
		if (diffuse_light[2] >= 1.0) diffuse_light[2] = 1.0;
		emission_light[0] += .1;
		if (emission_light[2] >= 1.0) emission_light[2] = 1.0;
		ambient_light[2] += .1;
		if (ambient_light[2] >= 1.0) ambient_light[2] = 1.0;
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, diffuse_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, diffuse_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, emission_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		break;
	case 'b':
		diffuse_light[2] -= .1;
		if (diffuse_light[2] <= 0.0) diffuse_light[2] = 0.0;
		emission_light[2] -= .1;
		if (emission_light[2] <= 0.0) emission_light[2] = 0.0;
		ambient_light[2] -= .1;
		if (ambient_light[2] <= 0) ambient_light[2] = 0.0;
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, diffuse_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, diffuse_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, emission_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		break;
	case 'p':case 'P':   //Perspective projection 
		SetOrthoGroundView();
		bPerspectiveMode = 1;
		bTopView = 0;
		reshape(viewportWidth, viewportHeight);
		break;
	case 'o':case 'O':   //Perspective projection 
		SetOrthoGroundView();
		break;
	case ' ':
		animationActive = -animationActive;
		if (animationActive > 0) {
			EnableTopView();
			eye[cX] = .5;
			eye[cY] = -.453;
			eye[cZ] = 1.211;
			center[cX] = .5;
			center[cY] = .5;
			center[cZ] = 2.0;
			//ViewAngleY = 1.0;
			orthoLeft = -.720;
			orthoRight = .72;
			orthoBottom = -.72;
			orthoTop = .72;
			orthoNear = -1.5;
			orthoFar = 1.5;
			bTopView = 1;
			//ViewAngleX = 55;
			glutIdleFunc(AnimateOverhead);
		}else{
			ViewAngleY = 0.0;
			bTopView = 0;
			glutIdleFunc(NULL);
		}
		break;
	case 'Q':	case 'q': exit(1);
	}

	printf("eX=%.3f eY=%.3f eZ=%.3f cX=%.3f cY=%.3f cZ=%.3f A=%.1F\n", eye[cX], eye[cY], eye[cZ], center[cX], center[cY], center[cZ], ViewAngleZ);

	glutPostRedisplay();
}

void updateManLocation(GLfloat x, GLfloat y) {
	ManPosX = x;
	ManPosY = y;
}

void SetOrthoGroundView() {
	ViewAngleX = -90; 
	bPerspectiveMode = 0;
	bTopView = 0;
	eye[cX] = ManPosX;
	eye[cY] = ManPosY;
	eye[cZ] = 0.066;
	center[cX] = ManPosX;
	center[cY] = ManPosY + .1;
	center[cZ] = 0.066;
	up[cX] = 0;
	up[cY] = 1;
	up[cZ] = 0;
	orthoLeft = -.1;
	orthoRight = .1;
	orthoBottom = -.1;
	orthoTop = .1;
	orthoNear = -.013;
	orthoFar = 1.5;
	reshape(viewportWidth, viewportHeight);
}

void SetManGridValues(int x, int y) {
	ManGridX = x;
	ManGridY = y;
}

void EnableTopView() {
	bPerspectiveMode = 0;
	glutSetWindowTitle("Top View");
	glPolygonMode(GL_FRONT, GL_FILL);
	glShadeModel(GL_SMOOTH);
	SetWallTextureOn();
	ViewAngleX = 0; ViewAngleY = 0; ViewAngleZ = 0;
	eye[cX] = .5;
	eye[cY] = .5;
	eye[cZ] = 2.0;
	center[cX] = .5;
	center[cY] = .5;
	center[cZ] = 2.0;
	up[cX] = 0;
	up[cY] = 1;
	up[cZ] = 0;
	orthoLeft = ORTHOLEFTSTART;
	orthoRight = ORTHORIGHTSTART;
	orthoBottom = ORTHOBOTTOMSTART;
	orthoTop = ORTHOTOPSTART;
	orthoNear = ORTHONEARSTART;
	orthoFar = ORTHOFARSTART;
	bTopView = 1;
	light_position[cX] = -2.0, light_position[cY] = -2.0, light_position[cZ] = 0;
	reshape(viewportWidth, viewportHeight);
}