/*
Project 1 - on github
CS321
Mark Erickson
*/

// Source file including main method
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <fstream>

#include "Project1.h"

// In the future this program could need command line functions
//int main(int argc, char** argv)
int main()
{
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
	return 0;
}
