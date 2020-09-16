/*
Project 2A
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
void displayPartA(void);
void myKeyboardEvent(unsigned char key, int x, int y);
//Proj1
void PromptFileName();
void CreateArray();
void ReadDataBySpace();
void closeFile();
void FreeMem();
void other_init();
void timer(int);
#endif