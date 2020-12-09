#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include "FinalProject.h"

	//using namespace std;

	// march 5, 1933 = 1971 = April 9, 2020



int main(int argc, char** argv)
{
	init_window(argc, argv);
	other_init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboardKeys);
	glutReshapeFunc(reshape);
	glutMainLoop();
}

