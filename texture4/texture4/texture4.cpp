#pragma warning(disable : 4996)
/*  This program maps a ppm format textures onto the cube */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

void read_file(char* argv);

GLint Width, Height, ImgWidth, ImgHeight, max, flag = 1;
FILE* infile;
char dummy[80];

static GLfloat LR = -3.0, range = 1.0;
static GLfloat theta = 0;

GLubyte* TexBits; /* Texture pixmap */

GLfloat smallVertices[] = { -0.3,-0.3,-0.3,0.3,-0.3,-0.3,
		0.3,0.3,-0.3, -0.3,0.3,-0.3, -0.3,-0.3,0.3,
		0.3,-0.3,0.3, 0.3,0.3,0.3, -0.3,0.3,0.3 };

GLfloat bigVertices[] = { -1.2,-1.2,-1.2,1.2,-1.2,-1.2,
		1.2,1.2,-1.2, -1.2,1.2,-1.2, -1.2,-1.2,1.2,
		1.2,-1.2,1.2, 1.2,1.2,1.2, -1.2,1.2,1.2 };

void other_init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(LR, -LR, LR, -LR, LR, -LR);
	glMatrixMode(GL_MODELVIEW);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void smallDraw_cube(int v1, int v2, int v3, int v4)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); 	glVertex3fv(smallVertices + v1 * 3);		glTexCoord2f(0.0, range);	glVertex3fv(smallVertices + v2 * 3);
	glTexCoord2f(range, range);	glVertex3fv(smallVertices + v3 * 3);
	glTexCoord2f(range, 0.0);	glVertex3fv(smallVertices + v4 * 3);
	glEnd();
}

void bigDraw_cube(int v1, int v2, int v3, int v4)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); 	glVertex3fv(bigVertices + v1 * 3);
	glTexCoord2f(0.0, range);	glVertex3fv(bigVertices + v2 * 3);
	glTexCoord2f(range, range);	glVertex3fv(bigVertices + v3 * 3);
	glTexCoord2f(range, 0.0);	glVertex3fv(bigVertices + v4 * 3);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (flag == 0) read_file("SmallTile.ppm");
	else read_file("BigPebble.ppm");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImgWidth, ImgHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, TexBits);

	glLoadIdentity();
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);

	glPushMatrix();
	glTranslatef(1.8, 1.8, 0.0);
	smallDraw_cube(0, 3, 2, 1);		smallDraw_cube(4, 5, 6, 7);
	smallDraw_cube(0, 4, 7, 3);		smallDraw_cube(1, 2, 6, 5);
	smallDraw_cube(2, 3, 7, 6);		smallDraw_cube(0, 1, 5, 4);

	glPopMatrix();
	glTranslatef(-0.7, -0.7, 0.0);
	bigDraw_cube(0, 3, 2, 1);		bigDraw_cube(4, 5, 6, 7);
	bigDraw_cube(0, 4, 7, 3);		bigDraw_cube(1, 2, 6, 5);
	bigDraw_cube(2, 3, 7, 6);		bigDraw_cube(0, 1, 5, 4);

	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'B': case 'b':
		flag = 1;		break;
	case 'S': case 's':
		flag = 0;		break;
	case 'N': case 'n':
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_NEAREST);
		break;
	case 'L': case 'l':
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
		break;
	case 'R': case 'r':
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case 'C': case 'c':
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		break;
	case 'I': case 'i':
		range += 0.2;
		printf("Texture range = %2.1f\n", range);
		break;
	case 'D': case 'd':
		range -= 0.2;
		printf("Texture range = %2.1f\n", range);
		break;
	case 'Q':	case 'q': exit(1);
	}
	glutPostRedisplay();
}

void read_file(char* argv)
{
	int i, temp;
	GLubyte* sp, c;

	if ((infile = fopen(argv, "rb+")) == NULL)
	{
		printf("File open error\n");
		exit(1);
	}
	do { temp = fgetc(infile); } while (temp != '\n');
	do { temp = fgetc(infile); } while (temp != '\n');		fscanf(infile, "%s", dummy);	ImgWidth = atoi(dummy);
	fscanf(infile, "%c", &c);
	fscanf(infile, "%s", dummy);	ImgHeight = atoi(dummy);
	fscanf(infile, "%c", &c);
	fscanf(infile, "%s", dummy);	max = atoi(dummy);
	fscanf(infile, "%c", &c);									// return after max

	if (TexBits != 0) free(TexBits);
	TexBits = (GLubyte*)calloc(ImgWidth * ImgHeight * 3, sizeof(GLubyte));
	for (i = ImgHeight - 1; i >= 0; i--)
	{
		sp = TexBits + i * ImgWidth * 3;
		fread(sp, sizeof(GLubyte), ImgWidth * 3, infile);
	}
	fclose(infile);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(LR, -LR, LR * (GLfloat)h / (GLfloat)w,
			-LR * (GLfloat)h / (GLfloat)w, LR, -LR);
	else
		glOrtho(LR * (GLfloat)w / (GLfloat)h,
			-LR * (GLfloat)w / (GLfloat)h, LR, -LR, LR, -LR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Width = w;	Height = h;
}

void sleep(clock_t wait)
{
	clock_t goal;
	goal = wait + clock();
	while (goal > clock());
}

void spinCube(void)
{
	theta += 5.0;
	if (theta > 360.0) theta -= 360.0;
	sleep((clock_t)(0.1 * CLOCKS_PER_SEC));
	glutPostRedisplay();
}

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	Width = Height = 600;
	TexBits = 0;
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("2D Textured Cube");
}

void main(int argc, char** argv)
{
	init_window(argc, argv);
	other_init();
	glutReshapeFunc(reshape);
	glutIdleFunc(spinCube);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
}
