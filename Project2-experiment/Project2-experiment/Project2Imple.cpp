/*
Project 2
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
#include "Project2.h"

std::ifstream fp;              // file pointer for input 
std::ofstream fpOut;           // file pointer for output 

std::string input_file_name;   // user to provide the name of the input file
std::string output_file_name; // system will calculate the output file
float* arrayPtr;              // name of the array storing the vertices
int arraySize=0;              // total size of the arrary
int arrayRowCount = 0;        // number of vertices (array's rows)
int arrayColCount = 3;        // number of coordinates on each line



void init_Window_Attrubutes(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Pre-Project #2 Experiment");
}

void other_init()
{
	glClearColor(0.2, 0.25, 0.3, 1);		/* Set background color */
	glMatrixMode(GL_PROJECTION);		/* Modify Projection Matrix */
	glLoadIdentity();				/* Set to identity matrix */
	glOrtho(0.0, 200.0, 200.0, 0.0, -1.0, 1.0);	/* Orthographic viewing volume */

//	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
	GLfloat i;
	GLfloat camera[3] = { 1,1,1 };

	//gluLookAt(camera[0], camera[1], camera[2], /* look from camera XYZ */
		//100, 95, 0, /* look at the origin */
		//0, 0, 1); /* positive Y up vector */

	glClear(GL_COLOR_BUFFER_BIT);	/* Clear color values */
	glColor3f(0.0, 0.0, 1.0);			/* Set foreground color */
	glPointSize(4.0);				/* Set point size */

	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2.0);				/* Set line width */

									/* Draw points */
	//glBegin(GL_POINTS);
	glBegin(GL_LINES);
	for (i = 0; i <= 100; i = i + 10) {
		glVertex3f(100, 30, 0);
		glVertex3f(50 + i, 80, 0);
	}
	glEnd();

	glColor3f(1.0, 1.0, 0.0);
	glLineWidth(9.0);				/* Set line width */

	glBegin(GL_LINES);
		glVertex3f(150, 95, 0);
		glVertex3f(50 , 95, 0);
	glEnd();

	for (i = 0; i < 360; i++) {
		myPolarPoint(100, 95, 40, i);
	}

	glColor3f(0.0, 0.0, 1.0);
	glLineWidth(7.0);				/* Set line width */

	/* Draw a line */
	glBegin(GL_LINES);
	for (i = 0; i <= 100; i = i + 10) {
		glVertex3f(50+i, 110, 0);
		glVertex3f(100, 190, 0);
	}
	glEnd();

	glRotatef(45.0, 0.f, 1.f, 0.f);/* orbit the Y axis */
/* ...where orbitDegrees is derived from mouse motion */


	glFlush();						/* Clear event buffer */
}

void myPolarPoint(GLfloat x, GLfloat y, GLfloat scale, GLfloat angle) {
	/*
	x, y  = center of polar coordinate
	angle, scale  = polar vector from x, y
	*/
	
	GLfloat i, x2, y2, radians;

	radians = angle * (GLfloat) (M_PI / 180);

	x2 = x + scale * cos(radians);
	y2 = y + scale * sin(radians);

	glBegin(GL_POINTS);
	for (i = 0; i < 100; i = i + 10) {}
		//glVertex3f(x, y, 0);
		glVertex3f(x2, y2, 0);
	
	glEnd();
}



//Proj1
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
	  //input_file_name = "img4.dat";

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
	arraySize = 0;       // Clear globals incase the user wants to run in a loop.
	arrayRowCount = 0;   //  basic clean up...
	arrayColCount = 3;
	fp.close();          // Be kind to your computer and look responsible.
	free(arrayPtr);
}

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
   arraySize = (arrayRowCount ) * (arrayColCount);
   arrayPtr = (float *) malloc(arraySize*8); // 4 bytes for each int, 8 for float
   printf("\nPlease stand by, there are %d lines to process in the input file.\n", arrayRowCount + JCount);
   printf("\n%d lines contain coordinate info.\n", arrayRowCount );
}

// Read data from the file to the dynamically allocated array
void ReadDataBySpace() {
	std::string line;
	std::string temp_line;
	std::string first_char_of_line;
	char single_char_in_line;
	char tempstr[256] = " ";
	int i = 0;
	unsigned int j = 0;

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
		if (first_char_of_line.compare("") == 0) {
			//printf("Found an empty line.\n");
		}
		else {
			// arrayPtr[i] and *(arrayPtr+i) can be used interchangeably 
			sscanf_s(line.c_str(), "%f %f %f", (arrayPtr + i * arrayColCount), (arrayPtr + i * arrayColCount + 1), (arrayPtr + i * arrayColCount + 2));
			// more debug stuff here
			//printf("i=%d array=X%.1f Y%.1f Z%.1f\n", i, *(arrayPtr + i * arrayColCount), *(arrayPtr + i * arrayColCount + 1), *(arrayPtr + i * arrayColCount + 2));
			//printf("arra=X%d Y%d Z%d\n", arrayPtr[i * arrayColCount], arrayPtr[i * arrayColCount + 1], arrayPtr[i * arrayColCount + 2]);
			//printf("\n\n");
			i++;
		}
	
	}
	//printf("Loop ends.\n");
}


// Reverse the order of the lines in the file and save with ".out" extension (and calculate the average )
void ProcessData()	{
	int i;
	char line_out[256];
	float current_x = 0, current_y = 0, current_z = 0;
	float mean_x = 0, mean_y = 0, mean_z = 0;

	i = input_file_name.length() - 3;    // this line to just make the next line shorter (and reuse the i)
	output_file_name = input_file_name.substr(0, i).append("out");
	printf("\nWriting %s\n", output_file_name.c_str());

	fpOut.open(output_file_name);

	//printf("Reversing... %d lines....\n", arrayRowCount);

	for (i = arrayRowCount-1; i >=0 ; i--) {
		/*  uncomment this area to see the progress on the console during execution.
		for (j = 0; j < 3; j++) {
			printf("\ti=%d j=%d \t%7.1f", i,j, *(arrayPtr + i * arrayColCount + j));
		}
		printf("\n");
		*/
		current_x = *(arrayPtr + i * arrayColCount + 0); // convert to float because memory is cheap
		current_y = *(arrayPtr + i * arrayColCount + 1); // makes it easier to read the lines below
		current_z = *(arrayPtr + i * arrayColCount + 2);
		mean_x = mean_x + current_x;                     // maintain sum to average later
		mean_y = mean_y + current_y;
		mean_z = mean_z + current_z;

		sprintf_s(line_out,"%7.1f %7.1f %7.1f", *(arrayPtr + i * arrayColCount + 0), *(arrayPtr + i * arrayColCount + 1), *(arrayPtr + i * arrayColCount + 2));
		fpOut << line_out << "\n";

	}
	sprintf_s(line_out, "Mean X-Coordinate = %7.1f", (mean_x / arrayRowCount));
	fpOut << line_out << "\n";
	sprintf_s(line_out, "Mean Y-Coordinate = %7.1f", (mean_y / arrayRowCount));
	fpOut << line_out << "\n";
	sprintf_s(line_out, "Mean Z-Coordinate = %7.1f", (mean_z / arrayRowCount));
	fpOut << line_out << "\n";
	fpOut.close();
}
