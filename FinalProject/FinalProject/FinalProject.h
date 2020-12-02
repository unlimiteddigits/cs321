#pragma once

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

void mazeFloor();
void other_init();
void display(void);
void reshape(int w, int h);
void init_window(int argc, char** argv);
void keyboard(unsigned char key, int x, int y);
void DoBackgroundStuff();