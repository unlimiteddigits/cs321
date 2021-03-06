/*
Project 2B
CS321
Mark Erickson
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
#include "Project2B.h"

std::ifstream fp;              // file pointer for input 
std::ofstream fpOut;           // file pointer for output 

std::string input_file_name;   // user to provide the name of the input file
std::string output_file_name; // system will calculate the output file
float* arrayPtr;              // name of the array storing the vertices
int arraySize=0;              // total size of the arrary
int arrayRowCount = 0;        // number of vertices (array's rows)
int arrayColCount = 4;        // number of coordinates on each line
float x_position = 0;
int iDirection = 1;
int iContinue = 1;
float xScale = 1.0, yScale = 1.0;
bool enlarge = true; // true is enlarge, false is shrink

float Distortion = 20.0;        // Starting point and reset value of distortion
float xDistortion = Distortion; // Starting point of distortion on the x axis
float yDistortion = Distortion; // Starting point of distortion on the y axis
float xSkewMultiplier, ySkewMultiplier;  // variables for a random number to create the flicket in the wind effect

struct vertex { GLfloat x, y, z; };
//struct vertex imgMapFromFile[];

void init_Window_Attrubutes(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(500, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Project 2B - West winds...");
}

void other_init()
{
	glClearColor(0.2f, 0.25f, 0.3f, 1.0f);	/* Set background color */
	glMatrixMode(GL_PROJECTION);		/* Modify Projection Matrix */
	glLoadIdentity();					/* Set to identity matrix */
	glOrtho(-2026.0, 2026.0, -2026.0, 2026.0, -1.0, 1.0);	/* Orthographic viewing volume */
	glutKeyboardFunc(myKeyboardEvent);
	glutCloseFunc(myCloseEvent);
	glutIdleFunc(DoBackgroundStuff);
	//glMatrixMode(GL_MODELVIEW);
}

void DoBackgroundStuff() {
	
	if (iDirection == 0)
	{
		//printf("Doing idle Stuff...\n"); 
		//glScalef(1/1.3, 1/1.3, 1);
		//glRotatef(10.0f, 0, 1.0, 0.0);

		//glutSwapBuffers();
		//Sleep(50);
	}
}

void FixViewport(int width, int height) {
	//find screensize and adjust viewport

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (height == 0)
		height = 1;
	double ratio = 1.0 * (width / height);
	ratio = .01;

	// Use the Projection Matrix
	//glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	//glLoadIdentity();

	// Set the viewport to be the entire window
	//glViewport(0, 0, width/5.0, height/5.0);
	//glViewport(-2026.0, 2026.0, -2026.0, 2026.0);
	// Set the correct perspective.
	//gluPerspective(0, ratio, -1000, 1000);
	//glOrtho(-2026.0, 2026.0, -2026.0, 2026.0, -1.0, 1.0);	/* Orthographic viewing volume */

	// Get Back to the Modelview
	//glMatrixMode(GL_MODELVIEW);
}

// callback for Part A of the assignment
void displayPartA(void)
{
	int i;
	GLfloat fX, fY, fZ;
	GLfloat prev_fX=0, prev_fY=0, prev_fZ=0;
	int fJump=0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear color values */
	glColor3f(0.0, 0.0, 1.0);		/* Set foreground color */
	glPointSize(4.0);				/* Set point size */
	//gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2.0);				/* Set line width */


	for (i = 0; i < arrayRowCount; i = i + 1) {
		fX = (GLfloat) * (arrayPtr + i * arrayColCount);
		fY = (GLfloat) * (arrayPtr + i * arrayColCount + 1);
		fZ = (GLfloat) * (arrayPtr + i * arrayColCount + 2);
		fJump = (int) *(arrayPtr + i * arrayColCount + 3);   // 4th dimension of the array used to flag where the "J" was in the file

		switch (fJump)
		{
		case 1:                                 // Start drawing on the next vertex  We just jumped here (J)
			glBegin(GL_LINES);
			prev_fX = fX;
			prev_fY = fY;
			break;
		case 2:                                 // Create the last line segment before another jump (J)
			glVertex3f(prev_fX, prev_fY, 0.0);
			glVertex3f(fX+ (xSkewMultiplier* xDistortion), fY + (ySkewMultiplier * yDistortion), 0.0);
			glEnd();
			break;
		default:                                // Normally just draw line segments and connect the dots.
			glVertex3f(prev_fX, prev_fY, 0.0);
			glVertex3f(fX+ (xSkewMultiplier * xDistortion), fY + (ySkewMultiplier * yDistortion), 0.0);
			prev_fX = fX;
			prev_fY = fY;
			break;
		}

	}

	//glFlush();						/* Clear event buffer */
	//glTranslatef(x_position, 0, 0);
	// glScalef(1.0f, 1.0f, 1.0f);  // scale on the x, y, and z axis
	//glScalef(xScale, yScale, 1.0f);
	//Sleep(50);
	glutSwapBuffers();
}

//for the exit using the keyboard
void myKeyboardEvent(unsigned char key, int x, int y)
{
	printf("User pressed %c\n", key);
	glutLeaveMainLoop();
}

//for the exit using the mouse click to X
void myCloseEvent()
{
	printf("Quiting after user pressed a key or Window X\n");
	iDirection = 0;
	iContinue = 0;
	//glutLeaveMainLoop();
}

void ScaleMe() {
	if (enlarge == true && xScale < 1.250f)
	{
		xScale = 1.1f;
		yScale = 1.1f;
	}
	else
	{
		enlarge = false; // Start Shrinking
	}
	if (enlarge == false && xScale > 0.55f)
	{
		xScale = 0.8f;
		yScale = 0.8f;
	}
	else
	{
		enlarge = true; // Start enlarging
	}
	//glLoadIdentity();
	glScaled(xScale, yScale, 1.0f);

	
}

//  for the animation which isn't a requirement of this part of the assignment
void timer(int n) {
	if (iDirection != 0) {
		glutPostRedisplay();
		//ScaleMe();
		glutTimerFunc(1000 / 60, timer, 0);
		xDistortion -= .1;
		yDistortion -= .1;
		if (xDistortion <= 0)
		{
			iDirection = 0;
			xDistortion = 0;
			yDistortion = 0;
		}
		xSkewMultiplier = rand() % 100;           // generate a number between 0 and 100
		ySkewMultiplier = (rand() % 100) - 50.0;  // generate a number between -50 and 50
		switch (iDirection)
		{
		case 1:
			if (x_position < 480)
				x_position += 5;
			else
				iDirection = -1;
			break;
		case -1:
			if (x_position > -500)
				x_position -= 5;
			else
				iDirection = 1; 
			break;
		default:
			break;
		}
	}else{
		if (iContinue == 1) {
			glRotatef(1, 0.0f, 0.0f, 1.0f);
			glutPostRedisplay();
			glutTimerFunc(1000 / 60, timer, 0);
			glutSetWindowTitle("Project 2B - The winds seems to have stopped.  Press any key to quit.");
		}
	}
}

void FreeMem() {
	arraySize = 0;       // Clear globals incase the user wants to run in a loop.
	arrayRowCount = 0;   //  basic clean up...
	arrayColCount = 4;
	iDirection = 1;
	iContinue = 1;
	xDistortion = Distortion;
	yDistortion = Distortion;
	free(arrayPtr);
}

//  -------------------------Everything below here is from project 1-------------------------
//  ---- Except the 4th dimension of the array used to flag where the "J" was in the file----
//  Search for "jumpFlag" in the ReadDataBySpace function to how the 4th dim was used.
//  This 4th dim is a key component in the "displayPartA" Callback.

//from Project1
// Prompt the file name
// Enter the file name: img1.dat
void PromptFileName()	{
   int flag = 1;
   char tempstr[256];

   while(flag)   {
      printf("Enter file name : ");

	  scanf_s("%s", &tempstr, 256);
	  //Flip comments on the following 2 lines to debug faster and add them to the line above.
	  input_file_name = tempstr;
	  //input_file_name = "img1.dat";

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
			if (jumpFlag == 1) {
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
			sscanf_s(line.c_str(), "%f %f %f", (arrayPtr + i * arrayColCount), (arrayPtr + i * arrayColCount + 1), (arrayPtr + i * arrayColCount + 2));
			// more debug stuff here
			//printf("i=%d array=X%.1f Y%.1f Z%.1f\n", i, *(arrayPtr + i * arrayColCount), *(arrayPtr + i * arrayColCount + 1), *(arrayPtr + i * arrayColCount + 2));
			//printf("arra=X%f Y%f Z%f J%f\n", arrayPtr[i * arrayColCount], arrayPtr[i * arrayColCount + 1], arrayPtr[i * arrayColCount + 2], arrayPtr[i * arrayColCount + 3]);
			//printf("\n\n");
			i++;
		}
	
	}
	//printf("Loop ends.\n");
	*(arrayPtr + (arrayRowCount-1) * arrayColCount + 3) = 2;
}

