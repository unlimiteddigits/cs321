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

GLfloat windowWidth = VIEWSTARTW;
GLfloat windowHeight = VIEWSTARTH;
GLfloat viewportWidth = VIEWSTARTW;
GLfloat viewportHeight = VIEWSTARTH;
GLfloat viewportXOffset = 0;
GLfloat viewportYOffset = 0;

GLdouble eye[3] = { 0.5,0.5,2.0 };
//GLdouble eye[3] = { 0.5,-0.5, -0.0 }; // Starting here doesn't
GLdouble center[3] = { 0.5,0.50,0 };
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


GLfloat ambient_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat diffuse_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };

GLfloat light_position[] = { -2.0, -2.0, 2.0, 1.0 };
GLfloat light_directional[] = { -2.0, 1.0, 2.0, 0.0 };

GLfloat r_diffuse_light[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat g_diffuse_light[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat b_diffuse_light[] = { 0.0, 0.0, 1.0, 1.0 };

int bTopView = 1;
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
	glutIdleFunc(DoBackgroundStuff);    // playing with more functions
	glClearColor(0.0, 0.0, 0.50, 1.0);
	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
		diffuse_light);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);         // Get Back to the Modelview
}

void DoBackgroundStuff() {

	GLfloat radians = 0;

	GLfloat	increment;
	increment = .066;
	GLfloat positionX = increment * 11 - (increment / 2);
	GLfloat positionY = -(increment / 2);
//	ManPosX = positionX;
//	ManPosY = positionY;

	if (bTopView==2)     // 
	{
		//printf("Doing idle Stuff...\n"); 
		bTopView = 0;
		for (double i = 0; i >= -90.0; i=i-5) {
			ViewAngleX = i;
			radians = (GLfloat)(ViewAngleX * (GLfloat)(M_PI / 180));
			eye[cZ] = (GLfloat)(0.10) * cos(radians);
			eye[cY] = (GLfloat)(0.10) * sin(radians);
			if (eye[cZ] < 0)
				up[cY] = -1.0;
			if (eye[cZ] >= 0)
				up[cY] = 1.0;
			display();
		}
		// like '+'/'-'
		orthoLeft = ManPosX-moveStep;
		orthoRight = ManPosX+moveStep;
		orthoBottom = ManPosY-moveStep;
		orthoTop = ManPosY+moveStep;
		 
		eye[cX] = ManPosX;
		eye[cY] = ManPosY-.1;
		eye[cZ] = .066;
		center[cX] = ManPosX;
		center[cY] = ManPosY;
		center[cZ] = .066;
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
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(eye[cX], -1.0, -0.032f);  //upper right
	glVertex3f(0, 0, -0.032f); //upper left
	glVertex3f(center[cX], center[cY], -0.010); //upper left
	glVertex3f(eye[cX], eye[cY], -0.032f);  //upper right
	glEnd();

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
	glViewport(0, 0, w/3, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w >= h)
		glOrtho(orthoLeft, orthoRight, 
			orthoBottom * (GLfloat)h / (GLfloat)w, orthoTop * (GLfloat)h / (GLfloat)w,
			orthoNear, orthoFar *2 );
	else
		glOrtho(orthoLeft * (GLfloat)w / (GLfloat)h, orthoRight * (GLfloat)w / (GLfloat)h,
			orthoBottom, orthoTop, orthoNear, orthoFar * 2);
	//glOrtho(-.010 * (GLfloat)w / (GLfloat)h, 1.01 * (GLfloat)w / (GLfloat)h, -0.01, 1.01, 1, -1);
	//glOrtho(0, 1, 0, 1, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	printf("oL=%.3f oR=%.3f oB=%.3f oT=%.3f oN=%.3f oF=%.3f w=%d h=%d\n", orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar,w, h);
}


void specialKeyboardKeys(int key, int x, int y) {
	printf("Speacial Key Pressed = %d  => ", key);
	switch (key)
	{
	case GLUT_KEY_UP:
		printf("Arrow Up\n");

		ManGridY += 1;
		moveToGridPos(ManGridX, ManGridY);
		break;
	case GLUT_KEY_DOWN:
		printf("Arrow Down\n");
		ManGridY -= 1;
		moveToGridPos(ManGridX, ManGridY);
		break;
	case GLUT_KEY_LEFT:
		printf("Arrow Left %.1f\n", ViewAngleZ);
		ManGridX -= 1;
		moveToGridPos(ManGridX, ManGridY);
		break;
	case GLUT_KEY_RIGHT:
		printf("Arrow Right %.1f\n",ViewAngleZ);
		ManGridX += 1;
		moveToGridPos(ManGridX, ManGridY);
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
}

void keyboard(unsigned char key, int x, int y)
{
	GLfloat radians = 0;

	//printf("Key Pressed = %c\n", key);
	switch (key)
	{
	case 'X': ViewAngleX += 5;
		radians = (GLfloat)(ViewAngleX * (GLfloat)(M_PI / 180));
		eye[cZ] = (GLfloat)(0.1) * cos(radians);
		eye[cY] = (GLfloat)(0.1) * sin(radians);
		if (eye[cZ] < 0)
			up[cY] = -1.0;
		if (eye[cZ] >= 0)
			up[cY] = 1.0;
/*		orthoLeft = ManPosX - moveStep;
		orthoRight = ManPosX + moveStep;
		orthoBottom = ManPosY - moveStep;
		orthoTop = ManPosY + moveStep;

		eye[cX] = ManPosX;
		eye[cY] = ManPosY - .1;
		eye[cZ] = .066;
		center[cX] = ManPosX - .5;
		center[cY] = ManPosY;
		center[cZ] = .066;
		up[cX] = 0;
		up[cY] = 0;
		up[cZ] = 1;
		reshape(viewportWidth, viewportHeight);
		*/
		break;
	case 'Y':
		//glPushMatrix();
		glTranslatef(-ManPosX, -ManPosY, -.066);
		glRotatef(5.0, 0, 1, 0);
		glTranslatef(ManPosX, ManPosY, .066);
		//glPopMatrix();
		/*
		ViewAngleY += 5;
		radians = (GLfloat)(ViewAngleY * (GLfloat)(M_PI / 180));
		eye[cZ] = (GLfloat)(0.1) * cos(radians);
		eye[cX] = (GLfloat)(0.1) * sin(radians);
		up[cY] = 1.0;
		*/
		break;
	case 'Z': ViewAngleZ += 5;
		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		eye[cX] = (GLfloat)(.1) * sin(radians);
		eye[cY] = (GLfloat)(.1) * cos(radians);
		break;
	case 'x': ViewAngleX -= 5;
		radians = (GLfloat)(ViewAngleX * (GLfloat)(M_PI / 180));
		eye[cZ] = (GLfloat)(.1) * cos(radians);
		eye[cY] = (GLfloat)(.1) * sin(radians);
		if (eye[cZ] < 0)
			up[cY] = (GLfloat)-1.0;
		if (eye[cZ] >= 0)
			up[cY] = (GLfloat)1.0;
		break;
	case 'y': ViewAngleY -= 5;
		radians = (GLfloat)(ViewAngleY * (GLfloat)(M_PI / 180));
		eye[cZ] = (GLfloat)(.1) * cos(radians);
		eye[cX] = (GLfloat)(.1) * sin(radians);
		up[cY] = 1.0;
		break;
	case 'z': ViewAngleZ -= 5;
		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		eye[cX] = (GLfloat)(.1) * sin(radians);
		eye[cY] = (GLfloat)(.1) * cos(radians);
		break;

	case 'L': 
		ViewAngleZ += 10.0;
		if (ViewAngleZ <= 0.0) ViewAngleZ = 360.0;

		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		center[cX] = ManPosX+(GLfloat)(.1) * sin(radians);
		center[cY] = ManPosY+ (GLfloat)(.1) * cos(radians);
		eye[cX] = ManPosX; eye[cY] = ManPosY;
		/*
		eye[cX] = 0.7; eye[cY] = -0.6; eye[cZ] = 0.033;
		center[cX] = 0.7; center[cY] = -0.5; center[cZ] = 0.033;
		up[cX] = 0; up[cY] = 0; up[cZ] = 1;
		*/
		printf("eX=%.3f eY=%.3f eZ=%.3f cX=%.3f cY=%.3f cZ=%.3f A=%.1F\n", eye[cX], eye[cY], eye[cZ], center[cX], center[cY], center[cZ], ViewAngleZ);
		break;
	case 'l': 
		ViewAngleZ -= 10.0;
		if (ViewAngleZ >= 360.0) ViewAngleZ = 0;

		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		center[cX] = ManPosX + (GLfloat)(.1) * sin(radians);
		center[cY] = ManPosY + (GLfloat)(.1) * cos(radians);
		eye[cX] = ManPosX; eye[cY] = ManPosY;
		/*
		eye[cX] = 0.7; eye[cY] = -0.6; eye[cZ] = 0.033;
		center[cX] = 0.7; center[cY] = -0.5; center[cZ] = 0.033;
		up[cX] = 0; up[cY] = 0; up[cZ] = 1;
		*/
		printf("eX=%.3f eY=%.3f eZ=%.3f cX=%.3f cY=%.3f cZ=%.3f A=%.1F\n", eye[cX], eye[cY], eye[cZ], center[cX], center[cY], center[cZ], ViewAngleZ);
		break;
		eye[cX] += moveStep; break; center[cX] += moveStep; break;
	case 'J': case 'j': eye[cX] -= moveStep; center[cX] -= moveStep; break;
	case 'I': case 'i': ManPosY += moveStep; break; // eye[cY] += moveStep; center[cY] += moveStep; break;
	case 'K': case 'k': ManPosY -= moveStep; break; // eye[cY] -= moveStep; center[cY] -= moveStep; break;
	case 'U': case 'u': eye[cZ] += moveStep; center[cZ] += moveStep; break;
	case 'V': case 'v': eye[cZ] -= moveStep; center[cZ] -= moveStep; break;
	case '-':
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
	case 'W': printf("Wireframe mode");  break;
	case '0':
		ViewAngleX = -90; ViewAngleY = 0; ViewAngleZ = 0;
		eye[cX] = ManPosX; 
		eye[cY] = ManPosY ; 
		eye[cZ] = 0;
		center[cX] = ManPosX;
		center[cY] = ManPosY + .1; 
		center[cZ] = 0.066;
		up[cX] = 0; 
		up[cY] = 1; 
		up[cZ] = 0;
		orthoLeft = ManPosX-.1;
		orthoRight = ManPosX+.1;
		orthoBottom = ManPosY -.1;
		orthoTop = ManPosY+.1;
		orthoNear = -.033;
		orthoFar = 1.5;

/*GLdouble eye[3] = { 0.5,0.5,2.0 };
//GLdouble eye[3] = { 0.5,-0.5, -0.0 }; // Starting here doesn't
GLdouble center[3] = { 0.5,0.50,0 };
GLdouble up[3] = { 0,1,0 };
*/
		bTopView = 1;
		light_position[cX] = -2.0, light_position[cY] = -2.0, light_position[cZ] = 0;
		reshape(viewportWidth, viewportHeight);
		break;
	case '2':
		light_position[cY] -= 1;
		light_directional[cY] -= 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		break;
	case '8':
		light_position[cY] += 1;
		light_directional[cY] += 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
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
		break;
	case '3':
		light_position[cZ] += 1;
		light_directional[cZ] += 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		break;
	case 'F': 	case 'f':
		glDisable(GL_LIGHT0);		break;
	case 'R': 	case 'r':
		glLightfv(GL_LIGHT0, GL_DIFFUSE, r_diffuse_light);
		glEnable(GL_LIGHT0);		break;
	case 'G': 	case 'g':
		glLightfv(GL_LIGHT0, GL_DIFFUSE, g_diffuse_light);
		glEnable(GL_LIGHT0);		break;
	case 'B': 	case 'b':
		glLightfv(GL_LIGHT0, GL_DIFFUSE, b_diffuse_light);
		glEnable(GL_LIGHT0);		break;
	case 'D': 	case 'd':
		glLoadIdentity();
		glLightfv(GL_LIGHT0, GL_POSITION, light_directional);	break;
	case 'P': 	case 'p':
		glLoadIdentity();
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);	break;
	case 'Q':	case 'q': exit(1);
	}


	glutPostRedisplay();
}

void updateManLocation(GLfloat x, GLfloat y) {
	ManPosX = x;
	ManPosY = y;
}