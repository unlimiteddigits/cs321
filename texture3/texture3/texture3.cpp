#pragma warning(disable : 4996)
/*  This program maps a ppm format texture onto the Teapot */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

void read_file(char* argv);

GLint Width, Height, ImgWidth, ImgHeight, max;   
FILE *infile;
char dummy[80];

static GLfloat LR = -3.0; 

GLubyte    *TexBits; /* Texture pixmap */

GLfloat  diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  light_position[] = {0.0, -5.0, 30.0, 1.0};

void other_init()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	read_file("Antique.ppm");
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImgWidth, ImgHeight, 0, GL_RGB,
					GL_UNSIGNED_BYTE, TexBits);

	glShadeModel (GL_SMOOTH);
	glLightfv(GL_LIGHT0,GL_AMBIENT,diffuse_light);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse_light);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    	glEnable(GL_TEXTURE_2D);
    	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	glPushMatrix();
    	glTranslatef(0.0, 0.0, -50.0);
    	glColor3f(1.0, 1.0, 1.0);
    	glutSolidTeapot(10.0);
    	glPopMatrix();
	glFlush();
   	glutSwapBuffers(); 
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'A': case 'a':
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
										GL_LINEAR);
			break;
		case 'R': case 'r':
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			break;
		case 'M': case 'm':
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			break;
		case 'Q':	case 'q': exit(1);
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
    Width  = w;
    Height = h;
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (float)Width / (float)Height, 0.1, 1000.0);
}

void init_window(int argc, char** argv)
{
    glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	Width = 800;	Height = 600;
	TexBits = 0;
	glutInitWindowSize(Width, Height); 
	glutInitWindowPosition(50, 50); 
    	glutCreateWindow("Texture Mapping #3");
}

void read_file(char* argv)
{
	int i, temp;
	GLubyte *sp, c;

	if ( (infile = fopen(argv, "rb+")) == NULL) 
	{
		printf("File open error\n");
		exit(1);
	}
	do	{	temp = fgetc(infile); }    while (temp != '\n');	
	do	{	temp = fgetc(infile); }    while (temp != '\n');		fscanf(infile, "%s", dummy);	ImgWidth = atoi(dummy);		
fscanf(infile, "%c", &c);
	fscanf(infile, "%s", dummy);	ImgHeight = atoi(dummy);	
fscanf(infile, "%c", &c);
	fscanf(infile, "%s", dummy);	max = atoi(dummy);
	fscanf(infile, "%c", &c);									// return after max

	if (TexBits != 0) free(TexBits);
	TexBits = (GLubyte *) calloc(ImgWidth * ImgHeight * 3, sizeof(GLubyte));
	for (i = ImgHeight - 1; i >= 0; i--)
	{
		sp = TexBits + i * ImgWidth * 3;
		fread (sp, sizeof(GLubyte), ImgWidth * 3, infile);
	} 
	fclose(infile);
}

void main(int argc, char** argv)
{
    	init_window(argc,argv);
	other_init();
    	glutReshapeFunc(reshape);
    	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
    	glutMainLoop();
}
