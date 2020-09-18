/*
Project 2B
CS321
Mark Erickson
*/

// Header file including interface of functions

//The value of M_PI will be needed to draw circles in the future
#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#ifndef EXAMPLE_H
#define EXAMPLE_H

void init_Window_Attrubutes(int argc, char** argv);
void displayPartA(void);
void myKeyboardEvent(unsigned char key, int x, int y);
void PromptFileName();
void CreateArray();
void ReadDataBySpace();
void closeFile();
void FreeMem();
void other_init();
void timer(int);
void FixViewport(int width, int height);
void DoBackgroundStuff();
void myCloseEvent();
#endif