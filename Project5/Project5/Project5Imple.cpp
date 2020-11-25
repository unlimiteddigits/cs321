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
int array_vn_ColCount = 3;
int array_ss_ColCount=1;
int array_ka_ColCount=3;
int array_kd_ColCount=3;
int array_ks_ColCount=3;
int array_ke_ColCount=3;
int array_se_ColCount=3;
int totalLineCount = 0;
static GLint width, height;
static GLfloat LR = -3.0;
float myNormalizer = 0;

GLfloat colors[] = { 0.9, 0.1, 0.1,			/*  back side - Red   */
					0.935, 0.35, 0.935,		/* top side - purple  */
					0.05, 0.95, 0.05,		/* Left Side - green  */
					0.065, 0.065, 0.95,		/* Right Side - blue  */
					.992, 1, 0,				/* Front Side - yellow*/
					0.995, 0.995, 0.995 };  /* Bottom Side - White*/

GLfloat ambient_light[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat emission_light[] = { 0.5, 0.5, 1.0, 1.0 };
GLfloat ambient_light_value = .6;
GLfloat emission_light_value = .6;

GLfloat diffuse_light[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat diffuse_light_value = .6;

GLfloat light_position[] = { -2.0, -2.0, 2.0, 1.0 };
GLfloat light_directional[] = { -2.0, 1.0, 2.0, 0.0 };

GLfloat r_diffuse_light[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat g_diffuse_light[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat b_diffuse_light[] = { 0.0, 0.0, 1.0, 1.0 };

GLfloat  specular_light[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  specular_property[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLint	 shininess = 5;
double LightAngleY = 0;

int iDirection = 1;           // Control of the timer / "direction of the wind"
int iContinue = 1;            //  stop the rotation after the user presses a key 
float xScale = 1.0, yScale = 1.0;  // Attempts to scale the image.
bool enlarge = true; // true is enlarge, false is shrink
int lastx=0, lasty=0; 
float angle = 0.0;
int mouseButtonState = 0;

const int cX = 0;
const int cY = 1;
const int cZ = 2;
int bLookAround = 1;

int Perspective_view = 1;
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize((int)windowWidth, (int)windowHeight);
	//x = (Screen Width - Window Width) , y = (Screen Height - Window Height) 
	glutInitWindowPosition((int) (glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2, (int) (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2);  // auto version -> 
	glutCreateWindow("Project 5");
}

void other_init()
{
	int w = windowWidth;
	int h = windowHeight;
	//glClearColor(0.2f, 0.25f, 0.3f, 1.0f);		/* Set background color black */
	glClearColor(0.9f, 0.925f, 0.93f, 1.0f);	/* Re-Set background color  white*/
	glMatrixMode(GL_PROJECTION);		/* Modify Projection Matrix */
	glLoadIdentity();					/* Set to identity matrix */
	glOrtho(-LR,LR,-LR,LR,-LR,LR);
	glutMouseFunc(myMouseEvent);		// run myMouseEvent when the user uses the mouse
	//glutPassiveMotionFunc(MouseMotionEvent);
	glutMotionFunc(MouseMotionEvent);

	glutKeyboardFunc(myKeyboardEvent);  // run myKeyboardEvent when the user presses a key
	glutCloseFunc(myCloseEvent);        // myCloseEvent set the flags needed to stop the timer function
	glutIdleFunc(DoBackgroundStuff);    // playing with more functions
	glMatrixMode(GL_MODELVIEW);         // Get Back to the Modelview
	glLoadIdentity();
	Perspective_view = 0;
	eye[cX] = 0; eye[cY] = 0; eye[cZ] = 0.0;
	center[cX] = 0; center[cY] = 0; center[cZ] = -3.0;
	up[cX] = 0;	up[cY] = 1;	up[cZ] = 0;
	bLookAround = 1;

	glShadeModel(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,diffuse_light);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(LR, -LR, LR * (GLfloat)h / (GLfloat)w,
			-LR * (GLfloat)h / (GLfloat)w, LR, -LR);
	else
		glOrtho(LR * (GLfloat)w / (GLfloat)h,
			-LR * (GLfloat)w / (GLfloat)h, LR, -LR, LR, -LR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	width = w;	height = h;
}

void DoBackgroundStuff() {
	
	if (iDirection == 0)     // 
	{
		//printf("Doing idle Stuff...\n"); 
	}
}

// callback for Part B of the assignment
void display(void)
{
	double centerX = (min_X + max_X) / 2;
	double centerY = (min_Y + max_Y) / 2;	double centerZ = (min_Z + max_Z) / 2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear color values */
	glColor3f(0.0, 0.0, 1.0);		/* Set foreground color */
	glPointSize(4.0);				/* Set point size */
	glColor3f(1.0, 0.0, 0.0);       // for the red - on blackish
	glLineWidth(2.0);				/* Set line width */
	
	if (bLookAround)
	{
		IndentifyMyTransformMatrix();
		printf("Viewport = %d\n", Perspective_view);
		Perspective_view = 1;
		if (Perspective_view)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			if (input_file_name == "teapot.obj") {
				glFrustum(-8, 8, -8, 8, 10, 50);
				gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0);
			}

			if (input_file_name == "alfaRomeo147.obj") {
				glFrustum(-25, 25, -25, 25, 20, 100);
				gluLookAt(0, 0, 115, 0, 0, 0, 0, 1, 0);
			}
			if (input_file_name == "cube.obj") {
				glFrustum(-2, 2, -2, 2, 2, 4);
				gluLookAt(0, 0, 4, 0, 0, 0, 0, 1, 0);
			}
			if (input_file_name == "gangster.obj") {
				glFrustum(-1.3, 1.3, -1.3, 1.3, 1, 10);
				gluLookAt(0, 0, 4, 0, 0, 0, 0, 1, 0);
			}
			if (input_file_name == "lamp.obj") {
				glFrustum(-7, 7, -7, 7, 7, 15);
				gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
			}
			//glTranslatef((float)centerX, (float)centerY, (float)centerZ);
			glMatrixMode(GL_MODELVIEW);
		}
		else
		{
			LR = -myNormalizer/windowHeight;
			if (myNormalizer / windowHeight < 4) {
				LR *= 4;
			}
			if (myNormalizer / windowHeight < 1) {
				LR = -3;
			}
			reshape(windowWidth, windowHeight);
			printf("max=%f\n", myNormalizer);
			//glScalef((myNormalizer*.2) / myNormalizer, (myNormalizer *.2) / myNormalizer, (myNormalizer *.2) / myNormalizer);
		}
		gluLookAt(eye[cX], eye[cY], eye[cZ], center[cX], center[cY], center[cZ], up[cX], up[cY], up[cZ]); // assume your eye is a 0,0,0
		bLookAround = 0;
	}
	//drawImgData();
	drawSolid();
	glFlush();						/* Clear event buffer */
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

void drawSolid() {
	int i, row, column;									// loop counter 
	GLfloat fX, fY, fZ;						// place holders for the vertices 
	int v1, n1, v2, n2, v3, n3, v4, n4;    // Variables for vertices / Texture / normal vectors.

	for (i = 0; i < Number_of_faces; i++) {
		v1 = arrayPtr_f[i * array_f_ColCount + 0]-1;
		v2 = arrayPtr_f[i * array_f_ColCount + 1] - 1;
		v3 = arrayPtr_f[i * array_f_ColCount + 2] - 1;
		n1 = arrayPtr_f[i * array_f_ColCount + 3] - 1;
		n2 = arrayPtr_f[i * array_f_ColCount + 4] - 1;
		n3 = arrayPtr_f[i * array_f_ColCount + 5] - 1;
		draw_triangle(v1,v2,v3,n1,n2,n3,1);
	}
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
	glRotatef((GLfloat)diffx, (sin(angle)), (cos(angle)), 0.0f);  // rotate the direction of the mouse on the screen, as apposed to the line below
	//glRotatef(diffx, (cos(angle)), (sin(angle)), 0.0f);  // Move about X when moving along X or rotate about X when moving left and right
	//printf("x=%d, y=%d, mouseButtonState=%d, lastx=%d, lasty=%d, angle=%f\n", x, y, mouseButtonState, lastx, lasty, angle);
}

//for Changing the view using the keyboard
void myKeyboardEvent(unsigned char key, int x, int y)
{
	double width = abs(min_X) + abs(max_X);
	double height = abs(min_Y) + abs(max_Y);
	double depth = abs(min_Z) + abs(max_Z);
	double centerX = (min_X + max_X) / 2.0;
	double centerY = (min_Y + max_Y) / 2.0;
	double centerZ = (min_Z + max_Z) / 2.0;
	float aspectRatio = (float)width / (float)height;
	GLfloat radians = 0;
	//float projectionMatrix[];
	height = 2;
	width = 2;

	switch (key) {
		// Move img up
		//  Enlarge the img 
	case '+':
		glScalef(1.1f, 1.1f, 1.1f);
		break;

		// shrink the img
	case '-':
		glScalef(0.9f, 0.9f, 0.9f);
		break;


		// Rotate 15 degrees in the Y positive axis
	case 'O':
		glTranslatef((float)centerX, (float)centerY, (float)centerZ);
		glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef((float)((-1.0) * centerX), (float)((-1.0) * centerY), (float)(((-1.0) * centerZ)));
		break;

		// Rotate 15 degrees in the Y negative axis
	case 'o':
		glTranslatef((float)centerX, (float)centerY, (float)centerZ);
		glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
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
	case 'S': 
		if (shininess < 126) shininess += 2;
		glMateriali(GL_FRONT, GL_SHININESS, shininess);
		break;
	case 's':
		if (shininess > 2) shininess -= 2;
		glMateriali(GL_FRONT, GL_SHININESS, shininess);
		break;

	case 'p':
		LightAngleY += 5;
		radians = (GLfloat)(LightAngleY * (GLfloat)(M_PI / 180));
		light_position[cZ] = (22) * cos(radians);
		light_position[cX] = (22) * sin(radians);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		break;

	case 'P':
		LightAngleY -= 5;
		radians = (GLfloat)(LightAngleY * (GLfloat)(M_PI / 180));
		light_position[cZ] = (22) * cos(radians);
		light_position[cX] = (22) * sin(radians);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		break;

		// If the 'q' or 'Q' key is pressed, the program exits.
	case '1':
		//glLoadIdentity();
		glLightfv(GL_LIGHT0, GL_POSITION, light_directional);
		glEnable(GL_LIGHT0);
		break;
	case '2':
		//glLoadIdentity();
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);	break;
		glEnable(GL_LIGHT0);
	case '0':
		glDisable(GL_LIGHT0);		break;
	case 'A':
		ambient_light_value *= 1.1;
		for (int ii = 0; ii < 3; ii++){
			ambient_light[ii] = ambient_light_value;
		}
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		break;
	case 'a':
		ambient_light_value *= .9;
		if (ambient_light_value < .1) {
			ambient_light_value = .1;
		}
		for (int ii = 0; ii < 3; ii++) {
			ambient_light[ii] = ambient_light_value;
		}
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
		break;
	case 'E':
		emission_light_value *= 1.1;
		for (int ii = 0; ii < 3; ii++) {
			emission_light[ii] = emission_light_value;
		}
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		break;
	case 'e':
		emission_light_value *= .9;
		if (emission_light_value < .1) {
			emission_light_value = .1;
		}
		for (int ii = 0; ii < 3; ii++) {
			emission_light[ii] = emission_light_value;
		}
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, emission_light);
		break;
	case 'd':
		diffuse_light_value *= 0.9;
		if (diffuse_light_value < .1) {
			diffuse_light_value = .1;
		}
		for (int ii = 0; ii < 3; ii++) {
			diffuse_light[ii] = diffuse_light_value;
		}
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
		glEnable(GL_LIGHT0);
		break;
	case 'D':
		diffuse_light_value *= 1.1;
		for (int ii = 0; ii < 3; ii++) {
			diffuse_light[ii] = diffuse_light_value;
		}
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
		glEnable(GL_LIGHT0);
		break;
	case 'q': case 'Q':
		printf("User pressed %c\n", key);
		glutLeaveMainLoop();   // Used instead of exit to allow looping in the main for selecting other img files.
		//exit(0);
	}

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
	//printf("eye_X=%f1.2,eye_Y=%f1.2, eye_Z=%f1.2, center_X=%f1.2, center_Y=%f1.2, center_Z=%f1.2\n", (eye[cX]), (eye[cY]), (eye[cZ]), (center[cX]), (center[cY]), (center[cZ]));
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
	Number_of_comments = 0; Number_of_object_names = 0; Number_of_group_names = 0;
	Number_of_vertices = 0; Number_of_normals = 0; Number_of_smoothshade = 0;
	Number_of_faces = 0; Number_of_usemtl = 0; Number_of_mtllib = 0; Number_of_se = 0;
	Number_of_ka = 0; Number_of_kd = 0; Number_of_ks = 0; Number_of_ke = 0;
	Number_of_numbers_on_line = 0;

	free(arrayPtr_v);
	free(arrayPtr_vn);
	free(arrayPtr_g);
	free(arrayPtr_f);
	free(arrayPtr_ka);
	free(arrayPtr_kd);
	free(arrayPtr_o);
	free(arrayPtr_se);
	free(arrayPtr_smoothShading);
	free(arrayPtr_usemtl);
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
	   else if (line.substr(0, 1) == "f") { Number_of_faces += 1;}
	   else if (line.substr(0, 1) == "#") { Number_of_comments += 1;}
	   else if (line.substr(0, 1) == "s") { Number_of_smoothshade += 1;}
	   else if (line.substr(0, 2) == "vn") {Number_of_normals += 1;}
	   else if (line.substr(0, 1) == "v") { Number_of_vertices += 1; }
	   else if (line.substr(0, 2) == "ka") {Number_of_ka += 1;}
	   else if (line.substr(0, 2) == "kd") {Number_of_kd += 1;}
	   else if (line.substr(0, 2) == "ks") {Number_of_ks += 1;}
	   else if (line.substr(0, 2) == "ke") {Number_of_ke += 1;}
	   else if (line.substr(0, 2) == "se") { Number_of_se += 1; }
	   else if (line.substr(0, 6) == "usemtl") { Number_of_usemtl += 1; }
	   else if (line.substr(0, 6) == "mtllib") { Number_of_mtllib += 1; }
	   else { printf("Unknown line found in file.  Line#%d\n", totalLineCount); }
   }
   arrayPtr_f = (float*)malloc(Number_of_faces * array_f_ColCount * 8); // 4 bytes for each int, 8 for float
   arrayPtr_o = (char*)malloc(Number_of_object_names * array_o_ColCount * 256);
   arrayPtr_g = (char*)malloc(Number_of_group_names * array_g_ColCount * 256);
   arrayPtr_usemtl = (char*)malloc(Number_of_usemtl * array_usemtl_ColCount * 64);
   arrayPtr_v = (float*)malloc((Number_of_vertices * array_v_ColCount) * 8);
   arrayPtr_vn = (float*)malloc((Number_of_normals * array_vn_ColCount) * 8);
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
	char prev_char_in_view=0;
	char tempstr[256] = " ";
	int line_length = 0;
	int i = 0, j = 0;
	int from_index = 0, to_index = 0;
	int jumpFlag = 0;
	float sum_X = 0, sum_Y = 0, sum_Z = 0;
	GLfloat temp_X = 0, temp_Y = 0, temp_Z = 0;
	GLfloat temp_U = 0, temp_V = 0, temp_W = 0;
	const int delimiter_max = 20;

	int delimiter[delimiter_max];
	int delimiter_index = 0;

	int ave_count = 0;

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
		//delimiter_index = 0 ;

		// loop to look for something other than numbers (and spaces).
		for (j = 0; j < line_length; j++) {
			char_in_view = (line.substr(j, 1).c_str()[0]);
			if (char_in_view == '#') {
				delimiter_index += 1;
				delimiter[delimiter_index] = j;
				delimiter_index += 1;
				delimiter[delimiter_index] = j+1;
				break;
			}else if (char_in_view == ' ') {
				if (prev_char_in_view != char_in_view) {
					delimiter_index += 1;
				}
				delimiter[delimiter_index] = j;
			}
			prev_char_in_view = char_in_view;
		}

		for (j = 0; j < delimiter_index; j++) {

		}
		obj_code = line.substr(0, delimiter[0]);
		if (obj_code.substr(0, 1) == "o") {
			Index_of_object_names += 1;
			////PutArrayVal_c(arrayPtr_o, Index_of_object_names, 0, line.substr(delimiter[1], (line_length - delimiter[1])));
		}
		else if (obj_code == "g") { 
			Index_of_group_names += 1; 
			//PutArrayVal_c(arrayPtr_g, Index_of_group_names, 0, line.substr(delimiter[1], (line_length - delimiter[1])));
		}
		else if (obj_code == "v" || obj_code == "v ") {
			Index_of_vertices += 1; 
			if (delimiter_index > 2) {
				temp_line = line.substr(delimiter[0] + 1, (delimiter[4] - delimiter[3]) - 1);
				sscanf_s(temp_line.c_str(), "%f %f %f %f", &temp_X, &temp_Y, &temp_Z, &temp_W);
			}
			else {
				temp_line = line.substr(delimiter[0] + 1, (line_length - delimiter[0]) - 1);
				sscanf_s(temp_line.c_str(), "%f %f %f", &temp_X, &temp_Y, &temp_Z);
			}
			*(arrayPtr_v + Index_of_vertices * array_v_ColCount + 0)= temp_X;
			*(arrayPtr_v + Index_of_vertices * array_v_ColCount + 1)= temp_Y;
			*(arrayPtr_v + Index_of_vertices * array_v_ColCount + 2) = temp_Z;
			/*printf("X=%f Y=%f Z=%f\n", 
				arrayPtr_v[Index_of_vertices * array_v_ColCount + 0], 
				arrayPtr_v[Index_of_vertices * array_v_ColCount + 1],
				arrayPtr_v[Index_of_vertices * array_v_ColCount + 2]);
				*/
		}
		else if (obj_code.substr(0,1) == "f") {
			Index_of_faces += 1; 
			temp_line = line.substr(delimiter[0] + 1, (line_length - delimiter[0]) - 1);
			for (i = 0; i < (line_length - delimiter[0]) - 1; i++) {
				if (temp_line.substr(i, 1) == "/") { temp_line.replace(i,1, " "); }
			}
			sscanf_s(temp_line.c_str(),
				"%f %f %f %f %f %f", &temp_X, &temp_U, &temp_Y, &temp_V, &temp_Z, &temp_W);
			*(arrayPtr_f + Index_of_faces * array_f_ColCount + 0) = temp_X;
			*(arrayPtr_f + Index_of_faces * array_f_ColCount + 1) = temp_Y;
			*(arrayPtr_f + Index_of_faces * array_f_ColCount + 2) = temp_Z;
			*(arrayPtr_f + Index_of_faces * array_f_ColCount + 3) = temp_U;
			*(arrayPtr_f + Index_of_faces * array_f_ColCount + 4) = temp_V;
			*(arrayPtr_f + Index_of_faces * array_f_ColCount + 5) = temp_W;
			/*
			printf("V1=%f f1=%f V2=%f f2=%f v3=%f f3=%f\n", 
				arrayPtr_f[Index_of_faces * array_f_ColCount + 0],
				arrayPtr_f[Index_of_faces * array_f_ColCount + 0+3],
				arrayPtr_f[Index_of_faces * array_f_ColCount + 1],
				arrayPtr_f[Index_of_faces * array_f_ColCount + 1+3],
				arrayPtr_f[Index_of_faces * array_f_ColCount + 2],
				arrayPtr_f[Index_of_faces * array_f_ColCount + 2+3]);
			*/
		}
		else if (obj_code.substr(0, 1) == " ") {
			// empty line
		}
		else if (obj_code.substr(0, 1) == "#") {
			Index_of_comments += 1;
		}
		else if (obj_code.substr(0, 1) == "s") {
			Index_of_smoothshade += 1; 
		}
		else if (obj_code.substr(0, 2) == "vn") {
			Index_of_normals += 1; 
			temp_line = line.substr(delimiter[0] + 1, (line_length - delimiter[0]) - 1);
			sscanf_s(temp_line.c_str(), "%f %f %f", &temp_X, &temp_Y, &temp_Z);
			*(arrayPtr_vn + Index_of_normals * array_vn_ColCount + 0) = temp_X;
			*(arrayPtr_vn + Index_of_normals * array_vn_ColCount + 1) = temp_Y;
			*(arrayPtr_vn + Index_of_normals * array_vn_ColCount + 2) = temp_Z;
			/*printf("nX=%f nY=%f nZ=%f\n",
				arrayPtr_vn[Index_of_normals * array_vn_ColCount + 0],
				arrayPtr_vn[Index_of_normals * array_vn_ColCount + 1],
				arrayPtr_vn[Index_of_normals * array_vn_ColCount + 2]);
				*/
		}
		else if (obj_code.substr(0, 2) == "ka") {
			Index_of_ka += 1;
		}
		else if (obj_code.substr(0, 2) == "kd") {
			Index_of_kd += 1;
		}
		else if (obj_code.substr(0, 2) == "ks") {
			Index_of_ks += 1;
		}
		else if (obj_code.substr(0, 2) == "ke") {
			Index_of_ke += 1; 
		}
		else if (obj_code.substr(0, 2) == "se") {
			Index_of_se += 1; 
		
		}
		else if (obj_code.substr(0, 6) == "usemtl") {
			Index_of_usemtl += 1; }
		else if (obj_code.substr(0, 6) == "mtllib") {
			Index_of_mtllib += 1; 
		}
		else { printf("Unknown line found in file.  Line#%d -> %s\n", i,line); }

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
		//}
	}


	//printf("Loop ends.\n");
	//*(arrayPtr_v + (totalLineCount -1) * arrayColCount + 3) = 2;
	average_X = sum_X / (float) ave_count;
	average_Y = sum_Y / (float) ave_count;
	average_Z = sum_Z / (float) ave_count;
	
	myNormalizer = abs(min_X);
	if (myNormalizer < abs(min_Y)) myNormalizer = abs(min_Y);
	if (myNormalizer < abs(min_Z)) myNormalizer = abs(min_Z);
	if (myNormalizer < abs(max_X)) myNormalizer = abs(max_X);
	if (myNormalizer < abs(max_Y)) myNormalizer = abs(max_Y);
	if (myNormalizer < abs(max_Z)) myNormalizer = abs(max_Z);
	//glScalef(1 / myNormalizer, 1 / myNormalizer, 1 / myNormalizer);
	max_X = max_X / myNormalizer;
	max_Y = max_Y / myNormalizer;
	max_Z = max_Z / myNormalizer;
	min_X = min_X / myNormalizer;
	min_Y = min_Y / myNormalizer;
	min_Z = min_Z / myNormalizer;
	j = i;
	for (i = 0; i < j; i++) {
		//printf("before i=%d array=X%.1f Y%.1f Z%.1f\n", i, *(arrayPtr + i * arrayColCount), *(arrayPtr + i * arrayColCount + 1), *(arrayPtr + i * arrayColCount + 2));
		arrayPtr_v[i * array_v_ColCount] = ((arrayPtr_v[i * array_v_ColCount])-min_X) / (max_X - min_X);
		arrayPtr_v[i * array_v_ColCount + 1] = ((arrayPtr_v[i * array_v_ColCount + 1])-min_Y) / (max_Y - min_Y);
		arrayPtr_v[i * array_v_ColCount + 2] = ((arrayPtr_v[i * array_v_ColCount + 2])-min_Z) / (max_Z - min_Z);
		//printf("after i=%d array=X%.4f Y%.4f Z%.4f\n", i, *(arrayPtr + i * arrayColCount), *(arrayPtr + i * arrayColCount + 1), *(arrayPtr + i * arrayColCount + 2));
	}

/*	printf("Average_X=%f", average_X);
	printf("Average_Y=%f", average_Y);
	printf("Average_Z=%f\n", average_Z);
	printf("Min_X=%f, Max_X=%f, Min_Y=%f, Max_Y=%f, Min_Z=%f, Max_Z=%f\n ", min_X, max_X, min_Y, max_Y, min_Z, max_Z );
*/
}


void PutArrayVal_f(float* arrayPtr,int Row, int Column, GLfloat value){
	*(arrayPtr + Row * arrayColCount + Column) = value;
}

float GetArrayVal(float* arrayPtr, int Column, int Row) {
	return  * (arrayPtr + Row * arrayColCount + Column);
}

void PutArrayVal_c(char* arrayPtr, int Column, int Row, char* value) {
	for (int i = 0; i < sizeof(value); i++) {
		arrayPtr[i] = value[i];
	}
	//arrayPtr[Row * arrayColCount + Column] = value;
}

void draw_triangle(int v1, int v2, int v3, int f1, int f2, int f3, int color)
{
	
	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
		colors + color * 3);

	glNormal3fv(arrayPtr_vn + f1 * array_vn_ColCount);
	glVertex3fv(arrayPtr_v + v1 * array_v_ColCount);
	glNormal3fv(arrayPtr_vn + f2 * array_vn_ColCount);
	glVertex3fv(arrayPtr_v + v2 * array_v_ColCount);
	glNormal3fv(arrayPtr_vn + f3 * array_vn_ColCount);
	glVertex3fv(arrayPtr_v + v3 * array_v_ColCount);
	glEnd();
}

void set_up_light(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, shininess);
	glEnable(GL_DEPTH_TEST);
}
