/*
Project 4
CS321
Mark Erickson
https://github.com/unlimiteddigits/cs321
*/

// Header file including interface of functions

//The value of M_PI will be needed to draw circles in the future
#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#ifndef EXAMPLE_H
#define EXAMPLE_H

void init_Window_Attrubutes(int argc, char** argv);
void display(void);
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
void drawString(float x, float y, float z, char* mystring);
void myMouseEvent(int button, int state, int x, int y);                //for extra credit
void MouseMotionEvent(int x, int y);
void TranslateMyTransformMatrix(GLfloat x, GLfloat y, GLfloat z);
void ScaleMyTransformMatrix(GLfloat myScaleX, GLfloat myScaleY, GLfloat myScaleZ);
void IndentifyMyTransformMatrix();
void RotateMyTransformMatrix(GLfloat myAngle, GLfloat x, GLfloat y, GLfloat z);
#endif