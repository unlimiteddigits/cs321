/*
Project 5
CS321
Mark Erickson
https://github.com/unlimiteddigits/cs321
*/

// Source file including implementation of functions

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <fstream>
#include <GL/freeglut.h>
#include "Project5.h"

std::ifstream fp;              // file pointer for input 
std::ofstream fpOut;           // file pointer for output 

std::string input_file_name;    // user to provide the name of the input file
std::string output_file_name;   // system will calculate the output file
float* arrayPtr_v;              // name of the array storing the vertices
float* arrayPtr_vn;             // name of the array storing the normals
float* arrayPtr_f;              // name of the array storing the faces
char* arrayPtr_usemtl;         // name of the array storing the usemtl
float* arrayPtr_smoothShading;  // name of the array storing the Smooth shading
float* arrayPtr_se;              // name of the array storing the faces
float* arrayPtr_ka;              // name of the array storing the ambient reflection coefficient
float* arrayPtr_kd;              // name of the array storing the diffuse reflection coefficient
float* arrayPtr_ks;              // name of the array storing the specular reflection coefficient
float* arrayPtr_ke;              // name of the array storing the material reflection coefficient
char* arrayPtr_o;              // name of the array storing the object name
char* arrayPtr_g;              // name of the array storing the group names
int array_f_ColCount=6; // 4 bytes for each int, 8 for float
int array_o_ColCount=3;
int array_g_ColCount=3;
int array_usemtl_ColCount=1;
int arrayColCount = 4;   // to be removed before turning in
int array_v_ColCount = 4;
int array_ss_ColCount=1;
int array_ka_ColCount=3;
int array_kd_ColCount=3;
int array_ks_ColCount=3;
int array_ke_ColCount=3;
int array_se_ColCount=3;
int totalLineCount = 0;

GLfloat colors[] = { 0.9, 0.1, 0.1,			/*  back side - Red   */
					0.935, 0.35, 0.935,		/* top side - purple  */
					0.05, 0.95, 0.05,		/* Left Side - green  */
					0.065, 0.065, 0.95,		/* Right Side - blue  */
					.992, 1, 0,				/* Front Side - yellow*/
					0.995, 0.995, 0.995 };  /* Bottom Side - White*/

int iDirection = 1;           // Control of the timer / "direction of the wind"
int iContinue = 1;            //  stop the rotation after the user presses a key 
float xScale = 1.0, yScale = 1.0;  // Attempts to scale the image.
bool enlarge = true; // true is enlarge, false is shrink
int lastx=0, lasty=0; 
float angle = 0.0;
int mouseButtonState = 0;

int cX = 0;
int cY = 1;
int cZ = 2;
int bLookAround = 1;

int Perspective_view = 0;
int multi_view_port = 0;

GLfloat windowWidth = 600;
GLfloat windowHeight = 400;
GLfloat viewportWidth = VIEWSTARTW;
GLfloat viewportHeight = VIEWSTARTH;
GLfloat viewportXOffset = 0;
GLfloat viewportYOffset = 0;

GLfloat Distortion = 1.0;        // Starting point and reset value of distortion.  Or... The maximum distance the "flame" will flicker in the wind
GLfloat xDistortion = Distortion; // Starting point of distortion on the x axis
GLfloat yDistortion = Distortion; // Starting point of distortion on the y axis
GLfloat xSkewMultiplier, ySkewMultiplier;  // variables for a random number to create the flicket in the wind effect

int Number_of_comments = 0, Number_of_object_names = 0, Number_of_group_names = 0;
int Number_of_vertices = 0, Number_of_normals = 0, Number_of_smoothshade = 0;
int Number_of_faces = 0, Number_of_usemtl = 0, Number_of_mtllib = 0, Number_of_se = 0;
int Number_of_ka = 0, Number_of_kd = 0, Number_of_ks = 0, Number_of_ke = 0;
int Number_of_numbers_on_line = 0;

int Index_of_comments = 0, Index_of_object_names = 0, Index_of_group_names = 0;
int Index_of_vertices = 0, Index_of_normals = 0, Index_of_smoothshade = 0;
int Index_of_faces = 0, Index_of_usemtl = 0, Index_of_mtllib = 0, Index_of_se = 0;
int Index_of_ka = 0, Index_of_kd = 0, Index_of_ks = 0, Index_of_ke = 0;
int Index_of_numbers_on_line = 0;

float average_X=0, average_Y=0, average_Z=0;
float max_X=0, max_Y=0, max_Z=0;
float min_X=0, min_Y=0, min_Z=0;
GLfloat prev_x = 0.0, prev_y=0.0, prev_z=0.0;
GLfloat myPrevScaleX=1.0, myPrevScaleY=1.0, myPrevScaleZ=1.0;

GLfloat totalAngle = 0;

GLdouble eye[3] = { 0,0,3.0 };
GLdouble center[3] = { 0,0,0 };
GLdouble up[3] = { 0,1,0 };

typedef GLfloat vertex4[4];
vertex4 myTransformMatrix[4] = {
						{1.0f, 0.0f, 0.0f, 0.0f},
						{0.0f, 1.0f, 0.0f, 0.0f},
						{0.0f, 0.0f, 1.0f, 0.0f},
						{0.0f, 0.0f, 0.0f, 1.0f} };



void init_Window_Attrubutes(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize((int)windowWidth, (int)windowHeight);
	//x = (Screen Width - Window Width) , y = (Screen Height - Window Height) 
	glutInitWindowPosition((int) (glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2, (int) (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2);  // auto version -> 
	glutCreateWindow("Project 5");
}


void other_init()
{
	//glClearColor(0.2f, 0.25f, 0.3f, 1.0f);		/* Set background color black */
	glClearColor(0.9f, 0.925f, 0.93f, 1.0f);	/* Re-Set background color  white*/
	glMatrixMode(GL_PROJECTION);		/* Modify Projection Matrix */
	glLoadIdentity();					/* Set to identity matrix */
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);	/* Orthographic viewing volume */
	glutMouseFunc(myMouseEvent);		// run myMouseEvent when the user uses the mouse
	//glutPassiveMotionFunc(MouseMotionEvent);
	glutMotionFunc(MouseMotionEvent);

	glutKeyboardFunc(myKeyboardEvent);  // run myKeyboardEvent when the user presses a key
	glutCloseFunc(myCloseEvent);        // myCloseEvent set the flags needed to stop the timer function
	glutIdleFunc(DoBackgroundStuff);    // playing with more functions
	glMatrixMode(GL_MODELVIEW);         // Get Back to the Modelview
	IndentifyMyTransformMatrix();
	//gluLookAt(eye[cX], eye[cY], eye[cZ], center[cX], center[cY], center[cZ], up[cX], up[cY], up[cZ]); // assume your eye is a 0,0,0
}


void DoBackgroundStuff() {
	
	if (iDirection == 0)     // 
	{
		//printf("Doing idle Stuff...\n"); 
	}
}

void FixViewport(int width, int height) {
	//find screensize and adjust viewport

}

// callback for Part B of the assignment
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear color values */
	glColor3f(0.0, 0.0, 1.0);		/* Set foreground color */
	glPointSize(4.0);				/* Set point size */
	glColor3f(1.0, 0.0, 0.0);       // for the red - on blackish
	glLineWidth(2.0);				/* Set line width */


	if (multi_view_port)
	{
		IndentifyMyTransformMatrix();
		glMatrixMode(GL_MODELVIEW);
		printf("Viewport = %d\n", Perspective_view);
		glViewport((GLsizei)0, (GLsizei)0, (GLint)viewportWidth, (GLint)viewportHeight);
		glLoadIdentity();
		//gluLookAt(0.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		gluLookAt(0, 0, 0, 0, 0, -3.0, 0, 1, 0); // 'O'
		drawImgData(); //display();
		glViewport((GLsizei)0, (GLsizei)(windowHeight - viewportHeight), (GLint)viewportWidth, (GLint)viewportHeight);
		glLoadIdentity();
		gluLookAt(0, 0, 0, 0, -3.0, 0, 0, 0, -1); // 'T'
		drawImgData(); //display();
		glViewport((GLsizei)(windowWidth - viewportWidth), (GLsizei)0, (GLint)viewportWidth, (GLint)viewportHeight);
		glLoadIdentity();
		gluLookAt(0, 0, 0, 3.0, 0, 0, 0, 1, 0); // 'S'
		drawImgData(); //display();
		glViewport((GLsizei)(windowWidth - viewportWidth), (GLsizei)(windowHeight - viewportHeight), (GLint)viewportWidth, (GLint)viewportHeight);
		//glLoadIdentity();
		//gluLookAt(0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
		//drawImgData(); //display();
	}
	else {
		drawBorder();
	}

	if (bLookAround)
	{
		IndentifyMyTransformMatrix();
		printf("Viewport = %d\n", Perspective_view);
		if (Perspective_view)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 4.0);
			glMatrixMode(GL_MODELVIEW);
		}
		else
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
		}
		gluLookAt(eye[cX], eye[cY], eye[cZ], center[cX], center[cY], center[cZ], up[cX], up[cY], up[cZ]); // assume your eye is a 0,0,0
		bLookAround = 0;
	}
	drawImgData();
	//glFlush();						/* Clear event buffer */
	//Sleep(50);                  // the timer is restricted below
	glutSwapBuffers();

}

void drawBorder() {
	glViewport((GLsizei)(((windowWidth - viewportWidth) / 2.0) + viewportXOffset), (GLsizei)(((windowHeight - viewportHeight) / 2.0) + viewportYOffset), (GLint)viewportWidth, (GLint)viewportHeight);
	glColor3f(1.0, 0.0, 0.0);       // for the Red - need multiple windows

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINE_STRIP);
	glVertex3f(1.0f, 1.0f, 0.0f);  //upper right
	glVertex3f(-1.0f, 1.0f, 0.0f); //upper left
	glVertex3f(-1.0f, -0.990f, 0.0f);  // lower left
	glVertex3f(0.990f, -0.99f, 0.0f); //Lower right
	glVertex3f(0.990f, 0.99f, 0.0f); //upper right
	glEnd();
	glPopMatrix(); glMatrixMode(GL_MODELVIEW); glPopMatrix();
}
void drawImgData() {
	int i, row, column;									// loop counter 
	GLfloat fX, fY, fZ;						// place holders for the vertices 
	GLfloat prev_fX = 0, prev_fY = 0, prev_fZ = 0;//remember the last point encountered in the array (place holers for simple reading- memory is cheap)
	int fJump = 0;                            // Flag for encountering the "J"ump
	char X_Label[] = "X-Axis";
	char Y_Label[] = "Y-Axis";
	char Z_Label[] = "Z-Axis";
	vertex4 myVertMatrix = { 0,0,0,0 };
	vertex4 myResultMatrix = { 0,0,0,0 };
	GLfloat test1 = 0, test2 = 0;

	glBegin(GL_LINES);
	for (i = 0; i < array_v_ColCount; i = i + 1) {
		fX = (GLfloat) * (arrayPtr_v + i * array_v_ColCount);
		fY = (GLfloat) * (arrayPtr_v + i * array_v_ColCount + 1);
		fZ = *(arrayPtr_v + i * array_v_ColCount + 2);
		myVertMatrix[0] = (GLfloat) * (arrayPtr_v + i * array_v_ColCount);
		myVertMatrix[1] = (GLfloat) * (arrayPtr_v + i * array_v_ColCount + 1);
		myVertMatrix[2] = *(arrayPtr_v + i * array_v_ColCount + 2);
		myVertMatrix[3] = 1;
		fJump = (int)*(arrayPtr_v + i * array_v_ColCount + 3);   // 4th dimension of the array used to flag where the "J" was in the file

		//myVert = myTransformMatrix * myVert;
		// Multiplying matrix a and b and storing in array mult.
		for (row = 0; row < 4; ++row) myResultMatrix[row] = 0;
		for (row = 0; row < 4; ++row)
		{
			for (column = 0; column < 4; ++column)
			{
				myResultMatrix[row] += myTransformMatrix[row][column] * myVertMatrix[column];
			}
			//test2 = 0;
		}
		fX = (GLfloat)myResultMatrix[0];
		fY = (GLfloat)myResultMatrix[1];
		fZ = (GLfloat)myResultMatrix[2];
		//fJump = (int) myResultMatrix[3];

		if (i > 447)
			fZ = (GLfloat)myResultMatrix[2];
		switch (fJump)
		{
		case 1:                                 // Start drawing on the next vertex  We just jumped here (J)
			prev_fX = fX;
			prev_fY = fY;
			prev_fZ = fZ;
			break;
		case 2:                                 // Create the last line segment before another jump (J)
			glVertex3f(prev_fX, prev_fY, prev_fZ);
			glVertex3f(fX + (xSkewMultiplier * xDistortion), fY + (ySkewMultiplier * yDistortion), fZ);
			break;
		default:                                // Normally just draw line segments and connect the dots.
			glVertex3f(prev_fX, prev_fY, prev_fZ);
			glVertex3f(fX + (xSkewMultiplier * xDistortion), fY + (ySkewMultiplier * yDistortion), fZ);
			prev_fX = fX;
			prev_fY = fY;
			prev_fZ = fZ;
			break;
		}

	}
	glEnd();
}

void drawString(float x, float y, float z, char* mystring) {
	glRasterPos3f(x, y, z);

	for (char* c = mystring; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);  // Updates the position
	}
}

//for Changing the view using the mouse
void myMouseEvent(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		//printf("Left Button ");
		if (state)
		{
			//printf("Up\n");
			mouseButtonState = 0;
		}
		else
		{
			//printf("Down\n");
			if (mouseButtonState==0)
			{
				//printf("Setting last xy\n");;
				mouseButtonState = 1;
				lastx = x; //set lastx to the current x position
				lasty = y; //set lasty to the current y position
			}
		}
		break;
	case GLUT_MIDDLE_BUTTON: printf("Middle Button %d\n", state); break;
	case GLUT_RIGHT_BUTTON:  printf("Right Button %d\n",state); break;
	}
	//printf("---------> x=%d, y=%d, button=%d, state=%d, lastx=%d, lasty=%d\n", x, y, button, state, lastx, lasty);
}

void MouseMotionEvent(int x, int y)
{
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);

	int diffx = x - lastx; //check the difference between the current x and the last x position
	int diffy = y - lasty; //check the difference between the current y and the last y position
	angle = atan((float) diffy/ (float) diffx);    // This is radian
	RotateMyTransformMatrix((GLfloat)diffx, (sin(angle)), (cos(angle)), 0.0f);  // rotate the direction of the mouse on the screen, as apposed to the line below
	//glRotatef((GLfloat)diffx, (sin(angle)), (cos(angle)), 0.0f);  // rotate the direction of the mouse on the screen, as apposed to the line below
	//glRotatef(diffx, (cos(angle)), (sin(angle)), 0.0f);  // Move about X when moving along X or rotate about X when moving left and right
	//printf("x=%d, y=%d, mouseButtonState=%d, lastx=%d, lasty=%d, angle=%f\n", x, y, mouseButtonState, lastx, lasty, angle);
}

//for Changing the view using the keyboard
void myKeyboardEvent(unsigned char key, int x, int y)
{
	double width = abs(min_X) + abs(max_X);
	double height = abs(min_Y) + abs(max_Y);
	double depth = abs(min_Z) + abs(max_Z);
	double centerX = (min_X + max_X) / 2;
	double centerY = (min_Y + max_Y) / 2;
	double centerZ = (min_Z + max_Z) / 2;
	float aspectRatio = (float)width / (float)height;
	//float projectionMatrix[];
	height = 2;
	width = 2;

	switch (key) {
		// Move img up
	case 'u':  //  object stays, viewport moves to simulate viewing wolume move
		glTranslatef(0.0f, (float)(height * (-.1)), 0.0f);
		viewportYOffset += (float)(viewportHeight * .1);
		break;
	case 'U':  // viewport stays, object moves
		//TranslateMyTransformMatrix(0.0f, (float)(height * .1), 0.0f);
		glTranslatef(0.0f, (float)(height * .1), 0.0f);
		break;

		// Move img left
	case 'l':   //  object stays, viewport moves to simulate viewing wolume move
		viewportXOffset += (float)(viewportWidth * (-0.1));
		glTranslatef((float)(width * .1), 0.0f, 0.0f);
		break;
	case 'L':  // viewport stays, object moves
		//TranslateMyTransformMatrix((float)-(width * .1), 0.0f, 0.0f);
		glTranslatef((float)-(width * .1), 0.0f, 0.0f);
		break;

		// Move img down
	case 'd':   //  object stays, viewport moves to simulate viewing wolume move
		glTranslatef(0.0f, (float) (height * .1), 0.0f);
		viewportYOffset += (float)(viewportHeight * -.1);
		break;
	case 'D':  // viewport stays, object moves
		//TranslateMyTransformMatrix(0.0f, (float)-(height * .1), 0.0f);
		glTranslatef(0.0f, (float)-(height * .1), 0.0f);
		break;

		// Move img right
	case 'r':   //  object stays, viewport moves to simulate viewing wolume move
		viewportXOffset += (float)(viewportWidth * .1);
		glTranslatef((float) (width * (-0.1)), 0.0f, 0.0f);
		break;
	case 'R':  // viewport stays, object moves
		//TranslateMyTransformMatrix((float)(width * .1), 0.0f, 0.0f);
		glTranslatef((float)(width * .1), 0.0f, 0.0f);
		break;
	case 'E':  // Enlarge viewport
		viewportWidth *= (GLfloat)1.1;
		viewportHeight *= (GLfloat)1.1;
		glScalef(0.9f, 0.9f, 0.9f);
		break;
	case 'e':  // lower case to lower viewport size
		if (viewportHeight>10.0)
		{
			glScalef(1.1f, 1.1f, 1.1f);
			viewportWidth *= (GLfloat)0.9;
			viewportHeight *= (GLfloat)0.9;
		}
		break;

		//  Enlarge the img 
	case '+':
		//ScaleMyTransformMatrix(1.1f, 1.1f, 1.1f);
		glScalef(1.1f, 1.1f, 1.1f);
		break;

		// shrink the img
	case '-':
		//ScaleMyTransformMatrix(0.9f, 0.9f, 0.9f);
		glScalef(0.9f, 0.9f, 0.9f);
		break;

		// Rotate 15 degrees in the X positive axis
	case 'X':
		//RotateMyTransformMatrix(15.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef((float)centerX, (float) centerY, (float) centerZ);
		glRotatef(15.0f,1.0f,0.0f,0.0f);
		glTranslatef((float) ((-1.0)*centerX), (float) ((-1.0)*centerY), (float) (((-1.0)*centerZ)));
		break;

		// Rotate 15 degrees in the X negative axis
	case 'x':
		//RotateMyTransformMatrix(-15.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef((float)centerX, (float)centerY, (float)centerZ);
		glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef((float)((-1.0) * centerX), (float)((-1.0) * centerY), (float)(((-1.0) * centerZ)));
		break;

		// Rotate 15 degrees in the X positive axis
	case 'Y':
		//RotateMyTransformMatrix(15.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef((float)centerX, (float)centerY, (float)centerZ);
		glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef((float)((-1.0) * centerX), (float)((-1.0) * centerY), (float)(((-1.0) * centerZ)));
		break;

		// Rotate 15 degrees in the X negative axis
	case 'y':
		//RotateMyTransformMatrix(-15.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef((float)centerX, (float)centerY, (float)centerZ);
		glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef((float)((-1.0) * centerX), (float)((-1.0) * centerY), (float)(((-1.0) * centerZ)));
		break;

		// Rotate 15 degrees in the X positive axis
	case 'Z':
		//RotateMyTransformMatrix(15.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef((float)centerX, (float)centerY, (float)centerZ);
		glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef((float)((-1.0) * centerX), (float)((-1.0) * centerY), (float)(((-1.0) * centerZ)));
		break;

		// Rotate 15 degrees in the X negative axis
	case 'z':
		//RotateMyTransformMatrix(-15.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef((float)centerX, (float)centerY, (float)centerZ);
		glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef((float)((-1.0) * centerX), (float)((-1.0) * centerY), (float)(((-1.0) * centerZ)));
		break;

		// This will reset the img back to its original position.
		// Since everything done is only stored in the state of openGL we can
		// simply reset the state to move the img back to its original position.
	case 'i': case 'I':
		IndentifyMyTransformMatrix();
		break;
	case 'v': case 'V':   // 4 viewport Perspective
		multi_view_port = 1;
		break;
	case 'p': case 'P':   // Perspective projection 
		multi_view_port = 0;
		Perspective_view = 1;
		eye[cX] = 0; eye[cY] = 0; eye[cZ] = 3.0;
		center[cX] = 0;	center[cY] = 0;	center[cZ] = 0;
		up[cX] = 0;	up[cY] = 1;	up[cZ] = 0;
		bLookAround = 1;
		break;
	case 'o':case 'O':   // Parallel orthographic projection, Front elevation
		IndentifyMyTransformMatrix();
		multi_view_port = 0;
		Perspective_view = 0;
		eye[cX] = 0; eye[cY] = 0; eye[cZ] = 0.0;
		center[cX] = 0; center[cY] = 0; center[cZ] = -3.0;
		up[cX] = 0;	up[cY] = 1;	up[cZ] = 0;
		bLookAround = 1;
		break;
	case 't': case 'T':    // Parallel orthographic projection, Top elevation
		IndentifyMyTransformMatrix();
		multi_view_port = 0;
		Perspective_view = 0;
		eye[cX] = 0; eye[cY] = 0;	eye[cZ] = 0;
		center[cX] = 0;	center[cY] = -3.0;	center[cZ] = 0;
		up[cX] = 0;	up[cY] = 0;	up[cZ] = -1;
		bLookAround = 1;
		break;
	case 'S': case 's':   // Parallel orthographic projection, side elevation
		IndentifyMyTransformMatrix();
		multi_view_port = 0;
		Perspective_view = 0;
		eye[cX] = 0;	eye[cY] = 0;	eye[cZ] = 0;
		center[cX] = 3.0;	center[cY] = 0;	center[cZ] = 0;
		up[cX] = 0;		up[cY] = 1;		up[cZ] = 0;
		bLookAround = 1;
		break;
	case 'f':  case 'F':  // Parallel orthographic projection, side elevation
		IndentifyMyTransformMatrix();
		if (center[cX]) eye[cX] += .1;
		if (center[cY]) eye[cY] += .1;
		if (center[cZ]) eye[cZ] += .1;
		bLookAround = 1;
		break;

	case 'N':  case 'n':  // Parallel orthographic projection, side elevation
		IndentifyMyTransformMatrix();
		if (center[cX]) eye[cX] -= .1;
		if (center[cY]) eye[cY] -= .1;
		if (center[cZ]) eye[cZ] -= .1;
		bLookAround = 1;
		break;

		// If the 'q' or 'Q' key is pressed, the program exits.
	case 'q': case 'Q':
		printf("User pressed %c\n", key);
		glutLeaveMainLoop();   // Used instead of exit to allow looping in the main for selecting other img files.
		//exit(0);
	}

}

void RotateMyTransformMatrix(GLfloat myAngle, GLfloat x, GLfloat y, GLfloat z)
{
	//glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);
	double myRadians= myAngle * 3.14159265 /180.0;
			
	totalAngle += (GLfloat)myRadians;

	myTransformMatrix[0][0] = (GLfloat) (x * x * (1.0 - cos(totalAngle)) * 1 + cos(totalAngle))* myPrevScaleX;
	myTransformMatrix[0][1] = (GLfloat)(x * y * (1.0 - cos(totalAngle)) * 1 - z * sin(totalAngle)) * myPrevScaleY;
	myTransformMatrix[0][2] = (GLfloat) (x * z * (1.0 - cos(totalAngle)) * 1 + y * sin(totalAngle))* myPrevScaleZ;
	myTransformMatrix[0][3] = prev_x;
	myTransformMatrix[1][0] = (GLfloat) (x * y * (1.0 - cos(totalAngle)) * 1 + z * sin(totalAngle))* myPrevScaleX;
	myTransformMatrix[1][1] = (GLfloat) (y * y * (1.0 - cos(totalAngle)) * 1 + cos(totalAngle))* myPrevScaleY;
	myTransformMatrix[1][2] = (GLfloat) (z * y * (1.0 - cos(totalAngle)) * 1 - x * sin(totalAngle))* myPrevScaleZ;
	myTransformMatrix[1][3] = prev_y;
	myTransformMatrix[2][0] = (GLfloat) (x * z * (1.0 - cos(totalAngle)) * 1 - y * sin(totalAngle))* myPrevScaleX;
	myTransformMatrix[2][1] = (GLfloat) (y * z * (1.0 - cos(totalAngle)) * 1 + x * sin(totalAngle))* myPrevScaleY;
	myTransformMatrix[2][2] = (GLfloat) (z * z * (1.0 - cos(totalAngle)) * 1 + cos(totalAngle))* myPrevScaleZ;
	myTransformMatrix[2][3] = prev_z; 
	myTransformMatrix[3][0] = 0;
	myTransformMatrix[3][1] = 0;
	myTransformMatrix[3][2] = 0;
	myTransformMatrix[3][3] = 1;

}

void TranslateMyTransformMatrix(GLfloat x, GLfloat y, GLfloat z)
{
	//should work like the built in glTranslatef(0.0f, (float)(height * .1), 0.0f);
	prev_x += x;
	prev_y += y;
	prev_z += z;

	myTransformMatrix[0][3] += x;
	myTransformMatrix[1][3] += y;
	myTransformMatrix[2][3] += z;
}

void ScaleMyTransformMatrix(GLfloat myScaleX, GLfloat myScaleY, GLfloat myScaleZ)
{
	// should work like the built in glScalef(1.1f, 1.1f, 1.1f);

	myPrevScaleX *= myScaleX;
	myPrevScaleY *= myScaleY;
	myPrevScaleZ *= myScaleZ;

	myTransformMatrix[0][0] *= myScaleX;
	myTransformMatrix[1][1] *= myScaleY;
	myTransformMatrix[2][2] *= myScaleZ;
	myTransformMatrix[3][3] = 1;
}

void IndentifyMyTransformMatrix()
{
	// should work like the built in glLoadIdentity();
	//  reset all variables and the matrix
	int row, column;

	for (row = 0; row < 4; ++row)
		for (column = 0; column < 4; ++column) myTransformMatrix[row][column] = 0;
	myTransformMatrix[0][0] = 1;
	myTransformMatrix[1][1] = 1;
	myTransformMatrix[2][2] = 1;
	myTransformMatrix[3][3] = 1;
	prev_x = 0;
	prev_y = 0;
	prev_z = 0;
	myPrevScaleX = 1.0;
	myPrevScaleY = 1.0;
	myPrevScaleZ = 1.0;
	totalAngle = 0.0;
	viewportXOffset = 0;
	viewportYOffset = 0;
	viewportWidth = VIEWSTARTW;
	viewportHeight = VIEWSTARTH;
	glLoadIdentity();
	printf("eye_X=%f1.2,eye_Y=%f1.2, eye_Z=%f1.2, center_X=%f1.2, center_Y=%f1.2, center_Z=%f1.2\n", (eye[cX]), (eye[cY]), (eye[cZ]), (center[cX]), (center[cY]), (center[cZ]));
}

//for the exit using the mouse click to X
void myCloseEvent()
{
	printf("Quiting after user pressed a key or Window X\n");
	iDirection = 0;
	iContinue = 0;
	IndentifyMyTransformMatrix();
	//glutLeaveMainLoop();
}

//  for the animation 
void timer(int n) {
	if (iDirection != 0) {
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, timer, 0); // 60 refreshes per second
		xDistortion -= .1f;                 // Reduce the reach of the "flame" in x
		yDistortion -= .1f;                 // Reduce the reach of the "flame" in y
		if (xDistortion <= 0)
		{
			iDirection = 0;
			xDistortion = 0;
			yDistortion = 0;
		}
		xSkewMultiplier = (GLfloat) (rand() % 100);           // generate a number between 0 and 100
		ySkewMultiplier = (GLfloat) (rand() % 100 - 50.0);    // generate a number between -50 and 50
	}else{
		if (iContinue == 1) {
			//glRotatef(1, 1.0f, 0.0f, 0.0f);                  // Rotate 1 deg at each time
			glutPostRedisplay();                           
			glutTimerFunc(1000 / 60, timer, 0);              // 60 refreshes per second
			glutSetWindowTitle("Project 3 - Press q to quit.");
		}
	}
}

void FreeMem() {
	totalLineCount = 0;   //  basic clean up...
	arrayColCount = 4;
	iDirection = 1;
	iContinue = 1;
	xDistortion = Distortion;   // Restore the max distortion value for the next loop
	yDistortion = Distortion;
	free(arrayPtr_v);
}


// Prompt the file name
// Enter the file name: img1.obj
void PromptFileName()	{
	int flag = 1;
	char tempstr[256];
	int iChoice = 0;

	while(flag)   {
		system("CLS");
		printf("\nProject 3 - Main menu.\n\n");
		printf("\tEnter 1 to load the file named alfaRomeo147.obj\n");
		printf("\tEnter 2 to load the file named cube.obj\n");
		printf("\tEnter 3 to load the file named gangster.obj\n");
		printf("\tEnter 4 to load the file named lamp.obj\n");
		printf("\tEnter 5 to load the file named teapot.obj\n");
		printf("\tEnter 6 to manually type the name of a file.\n\n");

		scanf_s("%s", &tempstr, 256);
		iChoice = atoi(tempstr);

		switch (iChoice)
		{
		case 1:
			input_file_name = "alfaRomeo147.obj";
			break;
		case 2:
			input_file_name = "cube.obj";
			break;
		case 3:
			input_file_name = "gangster.obj";
			break;
		case 4:
			input_file_name = "lamp.obj";
			break;
		case 5:
			input_file_name = "teapot.obj";
			break;
		default:
			printf("Enter file name : ");
			scanf_s("%s", &tempstr, 256);
			//Flip comments on the following 2 lines to debug faster and add them to the line above.
			input_file_name = tempstr;
			break;
		}

		fp.open(input_file_name);
		if (fp.is_open())
			flag = 0;
		else
		{
			printf("\nFile not Found.  Can't open %s!\n...Program is shutting down.\n", input_file_name.c_str());
			exit(0);
		}
	} 
}

void closeFile() {
	fp.close();          // Be kind to your computer and look responsible.
}

//from Project1
// Read information about the number of data in an input file
// Allocate a dynamic memory that will hold input data
void CreateArray()	{
	/*	Reference info for global variables
	int Number_of_comments = 0, Number_of_object_names = 0, Number_of_group_names = 0;
	int Number_of_vertices = 0, Number_of_normals = 0, Number_of_smoothshade = 0;
	int Number_of_faces = 0, Number_of_usemtl = 0, Number_of_se = 0;
	int Number_of_ka = 0, Number_of_kd = 0, Number_of_ks = 0, Number_of_ke = 0;
	int Number_of_numbers_on_line = 0;
	*/
	std::string line;
	char tempstr[256] = " ";
	int line_length = 0;
	totalLineCount = 0;
   while ( !fp.eof() ) {
	   fp.getline(tempstr,256);
	   line = tempstr;
	   totalLineCount += 1;
	   line_length = line.length();

	   // loop to look for something other than numbers (and spaces).

	   if (line.compare("") == 0) {
		   //printf("Found an empty line.\n");
	   }
	   else if (line.substr(0, 1) == "o") { Number_of_object_names += 1;}
	   else if (line.substr(0, 1) == "g") { Number_of_group_names += 1;}
	   else if (line.substr(0, 1) == "v") { Number_of_vertices += 1;}
	   else if (line.substr(0, 1) == "f") { Number_of_faces += 1;}
	   else if (line.substr(0, 1) == "#") { Number_of_comments += 1;}
	   else if (line.substr(0, 1) == "s") { Number_of_smoothshade += 1;}
	   else if (line.substr(0, 2) == "vn") {Number_of_normals += 1;}
	   else if (line.substr(0, 2) == "ka") {Number_of_ka += 1;}
	   else if (line.substr(0, 2) == "kd") {Number_of_kd += 1;}
	   else if (line.substr(0, 2) == "ks") {Number_of_ks += 1;}
	   else if (line.substr(0, 2) == "ke") {Number_of_ke += 1;}
	   else if (line.substr(0, 2) == "se") { Number_of_se += 1; }
	   else if (line.substr(0, 6) == "usemtl") { Number_of_usemtl += 1; }
	   else if (line.substr(0, 6) == "mtllib") { Number_of_mtllib += 1; }
	   else { printf("Unknown line found in file.  Line#%d", totalLineCount); }
   }
   arrayPtr_f = (float*)malloc(Number_of_faces * array_f_ColCount * 8); // 4 bytes for each int, 8 for float
   arrayPtr_o = (char*)malloc(Number_of_object_names * array_o_ColCount * 8);
   arrayPtr_g = (char*)malloc(Number_of_group_names * array_g_ColCount * 8);
   arrayPtr_usemtl = (char*)malloc(Number_of_usemtl * array_usemtl_ColCount * 8);
   arrayPtr_v = (float*)malloc(Number_of_vertices * array_v_ColCount * 8);
   arrayPtr_smoothShading = (float*)malloc(Number_of_smoothshade * array_ss_ColCount * 8);
   arrayPtr_ka = (float*)malloc(Number_of_ka * array_ka_ColCount * 8);
   arrayPtr_kd = (float*)malloc(Number_of_kd * array_kd_ColCount * 8);
   arrayPtr_ks = (float*)malloc(Number_of_ks * array_ks_ColCount * 8);
   arrayPtr_ke = (float*)malloc(Number_of_ke * array_ke_ColCount * 8);
   arrayPtr_se = (float*)malloc(Number_of_se * array_se_ColCount * 8);

   printf("\nPlease stand by, there are %d lines to process in the input file.\n", totalLineCount);
   printf("\n%d lines contain coordinate info.\n", Number_of_vertices);
}


//from Project1
// Read data from the file to the dynamically allocated array
void ReadDataBySpace() {
	std::string line;
	std::string temp_line;
	std::string first_char_of_line;
	std::string obj_code;

	char first_char_on_line;
	char second_char_on_line;
	char third_char_on_line;
	char char_in_view;
	char tempstr[256] = " ";
	int line_length = 0;
	int i = 0, j = 0;
	int from_index = 0, to_index = 0;
	int jumpFlag = 0;
	float sum_X = 0, sum_Y = 0, sum_Z = 0;
	GLfloat temp_X = 0, temp_Y = 0, temp_Z = 0;
	const int delimiter_max = 20;

	int delimiter[delimiter_max];
	int delimiter_index = 0;

	int ave_count = 0;
	float myNormalizer = 0;

	min_X = 0;
	max_X = 0;
	min_Y = 0;
	max_Y = 0;
	min_Z = 0;
	max_Z = 0;
	Index_of_group_names = -1; 
	Index_of_vertices = -1; 
	Index_of_faces = -1; 
	Index_of_comments = -1; 
	Index_of_smoothshade = -1; 
	Index_of_normals = -1;
	Index_of_ka = -1; 
	Index_of_kd = -1; 
	Index_of_ks = -1; 
	Index_of_ke = -1; 
	Index_of_se = -1; 
	Index_of_usemtl = -1; 
	Index_of_mtllib = -1; 

	fp.seekg(fp.beg);  // rewind the file or fp.seekg(0);

	if (!fp.good()) {
		fp.close();         // in case rewind doesn't work.
		fp.open(input_file_name);
	}

	while (!fp.eof()) {                    // loop to read line by line from file
		if (fp.good()) {
			fp.getline(tempstr, 256);
		}
		else {
			printf("Something wrong with %s", input_file_name.c_str());
		}
		line = tempstr;
		line_length = line.length();

		first_char_on_line = '\0';
		second_char_on_line = '\0';
		third_char_on_line = '\0';
		from_index = 0;
		to_index = 0;
		for (delimiter_index = delimiter_max-1; delimiter_index>=0; delimiter_index -= 1) {
			delimiter[delimiter_index] = -1;
		}
		// loop to look for something other than numbers (and spaces).
		for (j = 0; j < line_length; j++) {
			char_in_view = (line.substr(j, 1).c_str()[0]);
			if (char_in_view == ' ') {
				delimiter_index += 1;
				delimiter[delimiter_index] = j;
			}
		}

		for (j = 0; j < delimiter_index; j++) {

		}
		obj_code = line.substr(delimiter[j], (delimiter[j+1]-delimiter[j]));
		if (obj_code == "o") { 
			Index_of_object_names += 1;
			PutArrayVal(arrayPtr_o, Index_of_object_names, 0, line.substr(delimiter[1], (line_length - delimiter[1])));
		}
		else if (obj_code == "g") { 
			Index_of_group_names += 1; 
			PutArrayVal(arrayPtr_g, Index_of_group_names, 0, line.substr(delimiter[1], (line_length - delimiter[1])).c_str()[0]);
		}
		else if (obj_code == "v") { Index_of_vertices += 1; }
		else if (obj_code == "f") { Index_of_faces += 1; }
		else if (obj_code == "#") { Index_of_comments += 1; }
		else if (obj_code == "s") { Index_of_smoothshade += 1; }
		else if (obj_code == "vn") { Index_of_normals += 1; }
		else if (obj_code == "ka") { Index_of_ka += 1; }
		else if (obj_code == "kd") { Index_of_kd += 1; }
		else if (obj_code == "ks") { Index_of_ks += 1; }
		else if (obj_code == "ke") { Index_of_ke += 1; }
		else if (obj_code == "se") { Index_of_se += 1; }
		else if (obj_code == "usemtl") { Index_of_usemtl += 1; }
		else if (obj_code == "mtllib") { Index_of_mtllib += 1; }
		else { printf("Unknown line found in file.  Line#%d", totalLineCount); }



		if (j < 1) {
				if (line_length > 0) {
					first_char_on_line = (line.substr(j, 1).c_str()[0]);
				}
				if (line_length > 1) {
					second_char_on_line = (line.substr(j + 1, 1).c_str()[0]);
				}
				if (line_length > 2) {
					third_char_on_line = (line.substr(j + 2, 1).c_str()[0]);
				}
				if (second_char_on_line != ' ') {
					//for debugging try --> 
					printf("j=%d is -->%c%c%c<-- this.\n", j, first_char_on_line, second_char_on_line, third_char_on_line);
				}
			}    // end of if J=0

			if (isdigit(char_in_view) || char_in_view == '-' || from_index != 0) {
				// a 
				from_index = j;
			} else if (from_index > to_index && char_in_view == ' ') {
				to_index = j;
				//  printf("We need to truncate the string at %d........", j);
				temp_line = line.substr(0, j).c_str();
				line = temp_line;
				//printf("  shorter line=%s\n", line.c_str());
				break;
			}
		

		first_char_of_line = line.substr(0, 1);
		if (first_char_on_line=='J' || first_char_on_line == 'j') {
			jumpFlag = 1;
		}
		if (first_char_of_line.compare("") == 0) {
			//printf("Found an empty line.\n");
		}
		else {
			if (jumpFlag == 1 || i==0) {
				*(arrayPtr_v + i * arrayColCount + 3) = 1;
				jumpFlag = 0;
				if (i > 0) {
					*(arrayPtr_v + (i-1) * arrayColCount + 3) = 2;
				}
			}
			else {
				*(arrayPtr_v + i * arrayColCount + 3) = 0;
			}
			// arrayPtr[i] and *(arrayPtr_v+i) can be used interchangeably 

			sscanf_s(line.c_str(), "%f %f %f", &temp_X, &temp_Y, &temp_Z);
			//arrayPtr_v[i * arrayColCount] = temp_X ;
			PutArrayVal(arrayPtr_v, i, 0, temp_X);
			PutArrayVal(arrayPtr_v, i, 1, temp_Y);
			PutArrayVal(arrayPtr_v, i, 2, temp_Z);
			//arrayPtr_v[i * arrayColCount + 1]=temp_Y ;
			//arrayPtr_v[i * arrayColCount + 2]= temp_Z;

			if (temp_X > max_X) max_X = temp_X;
			if (temp_Y > max_Y) max_Y = temp_Y;
			if (temp_Z > max_Z) max_Z = temp_Z;
			if (temp_X < min_X) min_X = temp_X;
			if (temp_Y < min_Y) min_Y = temp_Y;
			if (temp_Z < min_Z) min_Z = temp_Z;

			sum_X += temp_X;
			sum_Y += temp_Y;
			sum_Z += temp_Z;
			ave_count++;

			// more debug stuff here
			//printf("i=%d array=X%.1f Y%.1f Z%.1f\n", i, *(arrayPtr + i * arrayColCount), *(arrayPtr + i * arrayColCount + 1), *(arrayPtr + i * arrayColCount + 2));
			//printf("arra=X%f Y%f Z%f J%f\n", arrayPtr[i * arrayColCount], arrayPtr[i * arrayColCount + 1], arrayPtr[i * arrayColCount + 2], arrayPtr[i * arrayColCount + 3]);
			//printf("\n\n");
			i++;
		}
	}


	//printf("Loop ends.\n");
	//*(arrayPtr_v + (totalLineCount -1) * arrayColCount + 3) = 2;
	average_X = sum_X / ave_count;
	average_Y = sum_Y / ave_count;
	average_Z = sum_Z / ave_count;
	
	myNormalizer = abs(min_X);
	if (myNormalizer < abs(min_Y)) myNormalizer = abs(min_Y);
	if (myNormalizer < abs(min_Z)) myNormalizer = abs(min_Z);
	if (myNormalizer < abs(max_X)) myNormalizer = abs(max_X);
	if (myNormalizer < abs(max_Y)) myNormalizer = abs(max_Y);
	if (myNormalizer < abs(max_Z)) myNormalizer = abs(max_Z);

	max_X = max_X / myNormalizer;
	max_Y = max_Y / myNormalizer;
	max_Z = max_Z / myNormalizer;
	min_X = min_X / myNormalizer;
	min_Y = min_Y / myNormalizer;
	min_Z = min_Z / myNormalizer;

	j = i;
	for (i = 0; i < j; i++) {
		//printf("before i=%d array=X%.1f Y%.1f Z%.1f\n", i, *(arrayPtr + i * arrayColCount), *(arrayPtr + i * arrayColCount + 1), *(arrayPtr + i * arrayColCount + 2));
		arrayPtr_v[i * arrayColCount] = arrayPtr_v[i * arrayColCount] / myNormalizer;
		arrayPtr_v[i * arrayColCount + 1] = arrayPtr_v[i * arrayColCount + 1] / myNormalizer;
		arrayPtr_v[i * arrayColCount + 2] = arrayPtr_v[i * arrayColCount + 2] / myNormalizer;
		//printf("after i=%d array=X%.4f Y%.4f Z%.4f\n", i, *(arrayPtr + i * arrayColCount), *(arrayPtr + i * arrayColCount + 1), *(arrayPtr + i * arrayColCount + 2));
	}

	printf("Average_X=%f", average_X);
	printf("Average_Y=%f", average_Y);
	printf("Average_Z=%f\n", average_Z);
	printf("Min_X=%f, Max_X=%f, Min_Y=%f, Max_Y=%f, Min_Z=%f, Max_Z=%f\n ", min_X, max_X, min_Y, max_Y, min_Z, max_Z );
}

void PutArrayVal(float* arrayPtr,int Column, int Row, GLfloat value){
	arrayPtr[Row * arrayColCount +Column] = value;
}

float GetArrayVal(float* arrayPtr, int Column, int Row) {
	return  * (arrayPtr + Row * arrayColCount + Column);
}

void draw_triangle(int v1, int v2, int v3, int color)
{
	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + color * 3);

	glNormal3fv(arrayPtr_vn + v1 * 3);
	glVertex3fv(arrayPtr_v + v1 * 3);
	glNormal3fv(arrayPtr_vn + v2 * 3);
	glVertex3fv(arrayPtr_v + v2 * 3);
	glNormal3fv(arrayPtr_vn + v3 * 3);
	glVertex3fv(arrayPtr_v + v3 * 3);
	//glNormal3fv(arrayPtr_vn + v4 * 3);
	//glVertex3fv(arrayPtr_v + v4 * 3);
	glEnd();
}
