/*
Project 2
CS321
Mark Erickson
*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

// Source file including main method
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <fstream>
#include <GL/freeglut.h>

#include "Project2.h"

// In the future this program could need command line functions
//int main()
int main(int argc, char** argv)
{

	PromptFileName();
	CreateArray();
	ReadDataBySpace();
	ProcessData();
	init_Window_Attrubutes(argc, argv);
	other_init();
	glutDisplayFunc(displayPartA);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	closeFile();
	//FreeMem();
	return 0;
	/*
		char answer;
		do {
			PromptFileName();
			CreateArray();
			ReadDataBySpace();
			ProcessData();
			closeFile();
			// repeat option for the grader's convenience.
			std::cout << std::endl << "You have reached this file's happy completion." << std::endl;
			std::cout << std::endl << "Would you like to try another file? [y/n]?";
			std::cin >> answer;
			std::cout << std::endl;
		} while (answer == 'Y' || answer == 'y');
		std::cout << std::endl << "You have reached this program's happy completion.  Good bye." << std::endl;
	*/
}
