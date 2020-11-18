/*	Shading Example #3 - Modified to move things around.
	Example of using glut objects */

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
//using namespace std;

// march 5, 1933 = 1971 = April 9, 2020

const int cX = 0, cY = 1, cZ = 2;

GLfloat Ex = 0.0, Ey = 0.0, Ez = 2.0,
Ax = 0.0, Ay = 0.0, Az = 0.0,
Ux = 0.0, Uy = 1.0, Uz = 0.0;
double ViewAngleX = 0;
double ViewAngleY = 0;
double ViewAngleZ = 0;
double AtAngleX = 0;
double AtAngleY = 0;
double AtAngleZ = 0;

static GLint width, height;
static GLfloat LR = -3.0;
static GLfloat theta = 0;

GLfloat ambient_light[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat diffuse_light[] = { 0.6, 0.6, 0.6, 1.0 };

GLfloat light_position[] = { -2.0, -2.0, 2.0, 1.0 };
GLfloat light_directional[] = { -2.0, 1.0, 2.0, 0.0 };

GLfloat r_diffuse_light[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat g_diffuse_light[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat b_diffuse_light[] = { 0.0, 0.0, 1.0, 1.0 };

void other_init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
		diffuse_light);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);         // Get Back to the Modelview
}

void change_view()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Ex, Ey, Ez, Ax, Ay, Az, Ux, Uy, Uz);
}

void drawString(float x, float y, float z, char* mystring) {
	glRasterPos3f(x, y, z);

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
	glLoadIdentity();
	glPushMatrix();
	change_view();

	DrawAxisLines();
	glTranslatef(-1.2, 1.2, 0.0);
	glutSolidTeapot(0.8);
	glTranslatef(2.0, -2.0, 0.0);
	glutSolidSphere(0.8, 20, 16);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
	//Status info
	sprintf_s(sResult, " Press x/X:  Ex=%0.1f Ey=%0.1f Ez=%0.1f \nAx=%0.1f Ay=%0.1f Az=%0.1f \nUx=%0.1f Uy=%0.1f Uz=%0.1f", (float)Ex, (float)Ey, (float)Ez, (float)Ax, (float)Ay, (float)Az, (float)Ux, (float)Uy, (float)Uz);
	drawString(-2.8, -2.5, 0, sResult);
	sprintf_s(sResult, " Light Position:  X=%0.1f Y=%0.1f Z=%0.1f (Numeric Keypad 2-8)  Plus u,v,w,r,g,b,d,f,p,i" , (float)light_position[cX], (float)light_position[cY], (float)light_position[cZ]);
	drawString(-2.8, -2.65, 0, sResult);

	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	GLfloat radians = 0;

	switch (key)
	{
	case 'X': ViewAngleX += 5; 	
		radians = (GLfloat)(ViewAngleX * (GLfloat)(M_PI / 180));
		Ez = (2.0) * cos(radians);
		Ey = (2.0) * sin(radians);
		if (Ez < 0)
			Uy = -1.0;
		if (Ez >= 0)
			Uy = 1.0;
		break;
	case 'Y': ViewAngleY += 5;  
		radians = (GLfloat)(ViewAngleY * (GLfloat)(M_PI / 180));
		Ez = (2.0) * cos(radians);
		Ex = (2.0) * sin(radians);
		Uy = 1.0;
		break;
	case 'Z': ViewAngleZ += 5;  		
		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		Ex = (2.0) * sin(radians);
		Ey = (2.0) * cos(radians);
		break;
	case 'x': ViewAngleX -= 5;	
		radians = (GLfloat)(ViewAngleX * (GLfloat)(M_PI / 180));
		Ez = (2.0) * cos(radians);
		Ey = (2.0) * sin(radians);
		if (Ez < 0)
			Uy = -1.0;
		if (Ez >= 0)
			Uy = 1.0;
		break;
	case 'y': ViewAngleY -= 5;	
		radians = (GLfloat)(ViewAngleY * (GLfloat)(M_PI / 180));
		Ez = (2.0) * cos(radians);
		Ex = (2.0) * sin(radians);
		Uy = 1.0;
		break;
	case 'z': ViewAngleZ -= 5;	
		radians = (GLfloat)(ViewAngleZ * (GLfloat)(M_PI / 180));
		Ex = (2.0) * sin(radians);
		Ey = (2.0) * cos(radians);
		break;
	case 'V': Ay += 5;  break;
	case 'v': Ay -= 5;	break;
	case 'U': Ax += 5;  break;
	case 'u': Ax -= 5;	break;
	case 'W': Az += 5;  break;
	case 'w': Az -= 5;	break;
	case 'i':case 'I': 
		Ex = 0; Ey = 0; Ez = 2.0, Ax = 0; Ay = 0; Az = 0, Ux = 0; Uy = 1; Uz = 0;
		ViewAngleX = 0, ViewAngleY = 0, ViewAngleZ = 0;
		light_position[cX] = -2.0, light_position[cY] = -2.0, light_position[cZ] = 0;
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

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(LR, -LR, LR * (GLfloat)h / (GLfloat)w,
			-LR * (GLfloat)h / (GLfloat)w, LR, -LR*2);
	else
		glOrtho(LR * (GLfloat)w / (GLfloat)h,
			-LR * (GLfloat)w / (GLfloat)h, LR, -LR, LR, -LR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	width = w;	height = h;
}

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	width = height = 500;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Move Light and Eye of Cam-Select RGB");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	other_init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMainLoop();
}

