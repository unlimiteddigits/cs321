/*
Project 5
CS321
Mark Erickson
https://github.com/unlimiteddigits/cs321
*/

// Source file including main method
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <fstream>
#include <GL/freeglut.h>
#include "Project5.h"

// In the future this program could need command line functions
// int main()
int main(int argc, char** argv)
{
	char answer='\0';

	do {
		PromptFileName();
		CreateArray();
		ReadDataBySpace();
		closeFile();
		init_Window_Attrubutes(argc, argv);
		other_init();
		set_up_light();
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
		glutReshapeFunc(reshape);
		glutDisplayFunc(display);
		glutTimerFunc(0, timer, 0);
		glutMainLoop();
		// repeat option for the grader's convenience.
		FreeMem();
		std::cout << std::endl << "You have reached this file's happy completion." << std::endl;
		std::cout << std::endl << "Would you like to try another file? [y/n]?";
		std::cin >> answer;
		std::cout << std::endl;
	} while (answer == 'Y' || answer == 'y');
	std::cout << std::endl << "You have reached this program's happy completion.  Good bye." << std::endl;
}
