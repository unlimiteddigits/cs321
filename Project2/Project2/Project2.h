/*
Project 2
CS321
Mark Erickson
*/

// Header file including interface of functions
#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#ifndef EXAMPLE_H
#define EXAMPLE_H

void init_Window_Attrubutes(int argc, char** argv);
void displaylogo(void);
void displayPartA(void);

//Proj1
void PromptFileName();
void CreateArray();
void ReadDataBySpace();
void ProcessData();
void closeFile();
void FreeMem();
void other_init();
void timer(int);
void myPolarPoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat scale);
#endif