
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

FILE *fp;

int *arrayPtr;
int arraySize;

void PromptFileName();
void CreateArray();
void ReadData();
void ProcessData();

// Prompt the file name
// Enter the file name: myData.dat
void PromptFileName()	{
   int flag = 1;
   char file_name[256];

   while(flag)   {
      printf("Enter file name : ");
      scanf("%s", file_name);
      if ((fp = fopen(file_name, "r")) == NULL)
		printf("\nCan't open %s!\n", file_name);
      else flag = 0;
   } 
}

// Read information about the number of data in an input file
// Allocate a dynamic memory that will hold input data
void CreateArray()	{
   char buffer[50];

   fgets(buffer, 50, fp);
   arraySize = atoi(buffer);

   printf("The number of data in the input file = %d\n", arraySize);
   arrayPtr = (int *) calloc(arraySize, sizeof(int));
}

// Read data from the file to the dynamically allocated array
void ReadData()	{
	int i;
	for (i = 0; i < arraySize; i++)		{
		fscanf(fp, "%d ", &arrayPtr[i]);
		printf("Array[%d] = %d\n", i, arrayPtr[i]);
	}
	printf("\n\n");
}


// Identify even number from data using a pointer
void ProcessData()	{
	int i, *sp;
	
	sp = arrayPtr;
	for (i = 0; i < arraySize; i++)	{
		if (((*sp) % 2) == 0) 
			printf("%d is an even number \n", *sp);
		sp++;
	}
}

void main(int argc, char** argv)
{
	char ch;

   	PromptFileName();
	CreateArray();
	ReadData();
	ProcessData();
	ch = getchar();	ch = getchar();
}