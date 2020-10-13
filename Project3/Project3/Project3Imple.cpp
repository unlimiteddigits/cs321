/*
Project 3
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
#include "Project3.h"

std::ifstream fp;              // file pointer for input 
std::ofstream fpOut;           // file pointer for output 

std::string input_file_name;   // user to provide the name of the input file
std::string output_file_name; // system will calculate the output file
float* arrayPtr;              // name of the array storing the vertices
int arraySize=0;              // total size of the arrary
int arrayRowCount = 0;        // number of vertices (array's rows)
int arrayColCount = 4;        // number of coordinates on each line
int iDirection = 1;           // Control of the timer / "direction of the wind"
int iContinue = 1;            //  stop the rotation after the user presses a key 
float xScale = 1.0, yScale = 1.0;  // Attempts to scale the image.
bool enlarge = true; // true is enlarge, false is shrink
int lastx=0, lasty=0; 
float angle = 0.0;
int mouseButtonState = 0;

GLfloat Distortion = 1.0;        // Starting point and reset value of distortion.  Or... The maximum distance the "flame" will flicker in the wind
GLfloat xDistortion = Distortion; // Starting point of distortion on the x axis
GLfloat yDistortion = Distortion; // Starting point of distortion on the y axis
GLfloat xSkewMultiplier, ySkewMultiplier;  // variables for a random number to create the flicket in the wind effect

float average_X=0, average_Y=0, average_Z=0;
float max_X=0, max_Y=0, max_Z=0;
float min_X=0, min_Y=0, min_Z=0;
int prev_x = 0, prev_y=0;

typedef GLfloat vertex4[4];
vertex4 myTransformMatrix[4] = {
						{1.0f, 0.0f, 0.0f, 0.0f},
						{0.0f, 1.0f, 0.0f, 0.0f},
						{0.0f, 0.0f, 1.0f, 0.0f},
						{0.0f, 0.0f, 0.0f, 1.0f} };



void init_Window_Attrubutes(int argc, char** argv) {
	int windowWidth = 500;
	int windowHeight = 400;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	//x = (Screen Width - Window Width) / 2, y = (Screen Height - Window Height) / 2
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2);
	glutCreateWindow("Project 3");
}

void other_init()
{
	glClearColor(0.2f, 0.25f, 0.3f, 1.0f);		/* Set background color black */
	glClearColor(0.9f, 0.925f, 0.93f, 1.0f);	/* Re-Set background color  white*/
	glMatrixMode(GL_PROJECTION);		/* Modify Projection Matrix */
	glLoadIdentity();					/* Set to identity matrix */
	glOrtho(-2026.0, 2026.0, -2026.0, 2026.0, -3526.0, 3526.0);	/* Orthographic viewing volume */
	glutMouseFunc(myMouseEvent);		// run myMouseEvent when the user uses the mouse
	//glutPassiveMotionFunc(MouseMotionEvent);
	glutMotionFunc(MouseMotionEvent);
	
	glutKeyboardFunc(myKeyboardEvent);  // run myKeyboardEvent when the user presses a key
	glutCloseFunc(myCloseEvent);        // myCloseEvent set the flags needed to stop the timer function
	glutIdleFunc(DoBackgroundStuff);    // playing with more functions
	glMatrixMode(GL_MODELVIEW);         // Get Back to the Modelview
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
void displayPartB(void)
{
	int i, row,column;									// loop counter 
	GLfloat fX, fY, fZ;						// place holders for the vertices 
	GLfloat prev_fX=0, prev_fY=0, prev_fZ=0;//remember the last point encountered in the array (place holers for simple reading- memory is cheap)
	int fJump=0;                            // Flag for encountering the "J"ump
	char X_Label[] = "X-Axis";
	char Y_Label[] = "Y-Axis";
	char Z_Label[] = "Z-Axis";
	vertex4 myVertMatrix;
	vertex4 myResultMatrix = { 0,0,0,0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear color values */
	glColor3f(0.0, 0.0, 1.0);		/* Set foreground color */
	glPointSize(4.0);				/* Set point size */
	glColor3f(1.0, 0.0, 0.0);       // for the red - on blackish
	glLineWidth(2.0);				/* Set line width */

	glBegin(GL_LINES);
	for (i = 0; i < arrayRowCount; i = i + 1) {
		myVertMatrix[0] = (GLfloat) * (arrayPtr + i * arrayColCount);
		myVertMatrix[1] = (GLfloat) * (arrayPtr + i * arrayColCount + 1);
		myVertMatrix[2] = (GLfloat) * (arrayPtr + i * arrayColCount + 2);
		myVertMatrix[3] =  *(arrayPtr + i * arrayColCount + 3);   // 4th dimension of the array used to flag where the "J" was in the file

		//myVert = myTransformMatrix * myVert;
		// Multiplying matrix a and b and storing in array mult.
		for (row = 0; row < 4; ++row) myResultMatrix[row] = 0;
		for (row = 0; row < 4; ++row)
			for (column = 0; column < 4; ++column)
				{
					myResultMatrix[row] += myTransformMatrix[row][column] * myVertMatrix[row];
				}

		fX = (GLfloat) myResultMatrix[0];
		fY = (GLfloat) myResultMatrix[1];
		fZ = (GLfloat) myResultMatrix[2];
		fJump = (int) myResultMatrix[3];

		switch (fJump)
		{
		case 1:                                 // Start drawing on the next vertex  We just jumped here (J)
//			glBegin(GL_LINES);
			prev_fX = fX;
			prev_fY = fY;
			prev_fZ = fZ;
			break;
		case 2:                                 // Create the last line segment before another jump (J)
			glVertex3f(prev_fX, prev_fY, prev_fZ);
			glVertex3f(fX+ (xSkewMultiplier* xDistortion), fY + (ySkewMultiplier * yDistortion), fZ);
//			glEnd();
			break;
		default:                                // Normally just draw line segments and connect the dots.
			glVertex3f(prev_fX, prev_fY, prev_fZ);
			glVertex3f(fX+ (xSkewMultiplier * xDistortion), fY + (ySkewMultiplier * yDistortion), fZ);
			prev_fX = fX;
			prev_fY = fY;
			prev_fZ = fZ;
			break;
		}

	}
	glEnd();
	glColor3f(0.0, 0.0, 1.0);       // for the blue - on blackish

	//glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);
	glEnd();
	
	//sprintf("", Axis_Label, "X-Axis");

	drawString(1000,100,0, X_Label);

	//Axis_Label = "Y-Axis";
	drawString(100, 1000, 0, Y_Label);

	//Axis_Label = "Z-Axis";
	drawString(100, 100, 1000, Z_Label);


	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
	//glDisable(GL_LINE_STIPPLE);

	glFlush();						/* Clear event buffer */
	//Sleep(50);                  // the timer is restricted below
	glutSwapBuffers();
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
	glRotatef((GLfloat) diffx, (sin(angle)), (cos(angle)), 0.0f);  // rotate the direction of the mouse on the screen, as apposed to the line below
	//glRotatef(diffx, (cos(angle)), (sin(angle)), 0.0f);  // Move about X when moving along X or rotate about X when moving left and right
	//printf("x=%d, y=%d, mouseButtonState=%d, lastx=%d, lasty=%d, angle=%f\n", x, y, mouseButtonState, lastx, lasty, angle);
}

//for Changing the view using the keyboard
void myKeyboardEvent(unsigned char key, int x, int y)
{
	double width = abs(min_X) + abs(max_X);
	double height = abs(min_Y) + abs(max_Y);
	double depth = abs(min_Z) + abs(max_Z);

	switch (key) {
		// Move img up
	case 'u': case 'U':
		TranslateMyTransformMatrix(0.0f, (float)(height * .1), 0.0f);
		//glTranslatef(0.0f, (float)(height * .1), 0.0f);
		break;

		// Move img left
	case 'l': case 'L':
		TranslateMyTransformMatrix((float)-(width * .1), 0.0f, 0.0f);
		//glTranslatef((float)-(width * .1), 0.0f, 0.0f);
		break;

		// Move img down
	case 'd': case 'D':
		TranslateMyTransformMatrix(0.0f, (float)-(height * .1), 0.0f);
		//glTranslatef(0.0f, (float)-(height * .1), 0.0f);
		break;

		// Move img right
	case 'r': case 'R':
		TranslateMyTransformMatrix((float)(width * .1), 0.0f, 0.0f);
		//glTranslatef((float)(width * .1), 0.0f, 0.0f);
		break;

		//  Enlarge the img 
	case '+':
		ScaleMyTransformMatrix(1.1f, 1.1f, 1.1f);
		//glScalef(1.1f, 1.1f, 1.1f);
		break;

		// shrink the img
	case '-':
		ScaleMyTransformMatrix(0.9f, 0.9f, 0.9f);
		//glScalef(0.9f, 0.9f, 0.9f);
		break;

		// Rotate 15 degrees in the X positive axis
	case 'X':
		glRotatef(15.0f,1.0f,0.0f,0.0f);
		break;

		// Rotate 15 degrees in the X negative axis
	case 'x':
		glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
		break;

		// Rotate 15 degrees in the X positive axis
	case 'Y':
		glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
		break;

		// Rotate 15 degrees in the X negative axis
	case 'y':
		glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
		break;

		// Rotate 15 degrees in the X positive axis
	case 'Z':
		glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
		break;

		// Rotate 15 degrees in the X negative axis
	case 'z':
		glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);
		break;

		// This will reset the img back to its original position.
		// Since everything done is only stored in the state of openGL we can
		// simply reset the state to move the img back to its original position.
	case 'i': case 'I':
		IndentifyMyTransformMatrix();
		//glLoadIdentity();
		break;

		// If the 'q' or 'Q' key is pressed, the program exits.
	case 'q': case 'Q':
		printf("User pressed %c\n", key);
		glutLeaveMainLoop();   // Used instead of exit to allow looping in the main for selecting other img files.
		//exit(0);
	}

}


void TranslateMyTransformMatrix(GLfloat x, GLfloat y, GLfloat z)
{
	//should work like the built in glTranslatef(0.0f, (float)(height * .1), 0.0f);
	myTransformMatrix[0][3] += x;
	myTransformMatrix[1][3] += y;
	myTransformMatrix[2][3] += z;
}

void ScaleMyTransformMatrix(GLfloat myScaleX, GLfloat myScaleY, GLfloat myScaleZ)
{
	// should work like the built in glScalef(1.1f, 1.1f, 1.1f);
	myTransformMatrix[0][0] = myTransformMatrix[0][0] * myScaleX;
	myTransformMatrix[1][1] = myTransformMatrix[1][1] * myScaleX;
	myTransformMatrix[2][2] = myTransformMatrix[2][2] * myScaleX;
}

void IndentifyMyTransformMatrix()
{
	// should work like the built in glLoadIdentity();
	int row, column;

	for (row = 0; row < 4; ++row)
		for (column = 0; column < 4; ++column) myTransformMatrix[row][column] = 0;
	myTransformMatrix[0][0] = 1;
	myTransformMatrix[1][1] = 1;
	myTransformMatrix[2][2] = 1;
	myTransformMatrix[3][3] = 1;
}

//for the exit using the mouse click to X
void myCloseEvent()
{
	printf("Quiting after user pressed a key or Window X\n");
	iDirection = 0;
	iContinue = 0;
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
	arraySize = 0;       // Clear globals incase the user wants to run in a loop.
	arrayRowCount = 0;   //  basic clean up...
	arrayColCount = 4;
	iDirection = 1;
	iContinue = 1;
	xDistortion = Distortion;   // Restore the max distortion value for the next loop
	yDistortion = Distortion;
	free(arrayPtr);
}

//  -------------------------Everything below here is from project 1-------------------------
//  ---- Except the 4th dimension of the array used to flag where the "J" was in the file----
//  ---- And the menu system which could have been in project 1
//  Search for "jumpFlag" in the ReadDataBySpace function to how the 4th dim was used.
//  This 4th dim is a key component in the "displayPartB" Callback.

//from Project1
// Prompt the file name
// Enter the file name: img1.dat
void PromptFileName()	{
	int flag = 1;
	char tempstr[256];
	int iChoice = 0;

	while(flag)   {
		system("CLS");
		printf("\nProject 3 - Main menu.\n\n");
		printf("\tEnter 1 to load the file named img1.dat\n");
		printf("\tEnter 2 to load the file named img2.dat\n");
		printf("\tEnter 3 to load the file named img3.dat\n");
		printf("\tEnter 4 to load the file named img4.dat\n");
		printf("\tEnter 5 to manually type the name of a file.\n\n");

		scanf_s("%s", &tempstr, 256);
		iChoice = atoi(tempstr);

		switch (iChoice)
		{
		case 1:
			input_file_name = "img1.dat";
			break;
		case 2:
			input_file_name = "img2.dat";
			break;
		case 3:
			input_file_name = "img3.dat";
			break;
		case 4:
			input_file_name = "img4.dat";
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
   std::string line;
   int JCount = 0;
   char tempstr[256]=" ";

   while ( !fp.eof() ) {
	   fp.getline(tempstr,256);
	   line = tempstr;
	   if (line.compare("") == 0) {
		   //printf("Found an empty line.\n");
	   }else if (line.substr(0,1)=="J" || line.substr(0, 1) == "j") {
		JCount++;
	   }
	   else {
		   if (line.substr(0, 1) == "j") {
			   JCount++;
		   }
		   else {
			   arrayRowCount++;
		   }
	   }
   }
   arraySize = (arrayRowCount) * (arrayColCount);
   arrayPtr = (float *) malloc(arraySize*8); // 4 bytes for each int, 8 for float
   printf("\nPlease stand by, there are %d lines to process in the input file.\n", arrayRowCount + JCount);
   printf("\n%d lines contain coordinate info.\n", arrayRowCount );
}


//from Project1
// Read data from the file to the dynamically allocated array
void ReadDataBySpace() {
	std::string line;
	std::string temp_line;
	std::string first_char_of_line;
	char single_char_in_line;
	char tempstr[256] = " ";
	int i = 0;
	unsigned int j = 0;
	int jumpFlag = 0;
	float sum_X = 0, sum_Y = 0, sum_Z = 0;
	GLfloat temp_X = 0, temp_Y = 0, temp_Z = 0;
	int ave_count = 0;

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

		// loop to look for something other than numbers (and spaces).
		for (j = 0; j < line.length(); j++) {
			single_char_in_line=(line.substr(j,1).c_str()[0]);
			//for debugging try --> printf("j=%d is -->%c<-- this.\n", j, single_char_in_line);
			if (isdigit(single_char_in_line) || single_char_in_line == '-' || single_char_in_line == ' ') {
				//This "line" (from from file) is still ok 
			} else { 
				//  printf("We need to truncate the string at %d........", j);
				temp_line = line.substr(0, j).c_str();
				line = temp_line;
				//printf("  shorter line=%s\n", line.c_str());
				break;
			}
		}

		first_char_of_line = line.substr(0, 1);
		if (single_char_in_line=='J' || single_char_in_line == 'j') {
			jumpFlag = 1;
		}
		if (first_char_of_line.compare("") == 0) {
			//printf("Found an empty line.\n");
		}
		else {
			if (jumpFlag == 1 || i==0) {
				*(arrayPtr + i * arrayColCount + 3) = 1;
				jumpFlag = 0;
				if (i > 0) {
					*(arrayPtr + (i-1) * arrayColCount + 3) = 2;
				}
			}
			else {
				*(arrayPtr + i * arrayColCount + 3) = 0;
			}
			// arrayPtr[i] and *(arrayPtr+i) can be used interchangeably 

			sscanf_s(line.c_str(), "%f %f %f", &temp_X, &temp_Y, &temp_Z);
			arrayPtr[i * arrayColCount] = temp_X ;
			arrayPtr[i * arrayColCount + 1]=temp_Y ;
			arrayPtr[i * arrayColCount + 2]= temp_Z;

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
	*(arrayPtr + (arrayRowCount-1) * arrayColCount + 3) = 2;
	average_X = sum_X / ave_count;
	average_Y = sum_Y / ave_count;
	average_Z = sum_Z / ave_count;
	printf("Average_X=%f", average_X);
	printf("Average_Y=%f", average_Y);
	printf("Average_Z=%f\n", average_Z);
	printf("Min_X=%f, Max_X=%f, Min_Y=%f, Max_Y=%f, Min_Z=%f, Max_Z=%f\n ", min_X, max_X, min_Y, max_Y, min_Z, max_Z );
}

