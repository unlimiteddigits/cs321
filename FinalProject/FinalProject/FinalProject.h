#pragma once

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#ifndef VIEWSTARTW
#define VIEWSTARTW 600
#endif

#ifndef VIEWSTARTH
#define VIEWSTARTH 500
#endif

#define ORTHOLEFTSTART -.5;
#define ORTHORIGHTSTART 0.5;
#define ORTHOBOTTOMSTART -.5;
#define ORTHOTOPSTART .5;
#define ORTHOFARSTART 1.5;
#define ORTHONEARSTART -1.5;

const int cX = 0, cY = 1, cZ = 2;

void mazeFloor(GLfloat ManPosX, GLfloat ManPosY, GLfloat ViewAngleX, GLfloat ViewAngleY, GLfloat ViewAngleZ);
void other_init();
void display(void);
void reshape(int w, int h);
void init_window(int argc, char** argv);
void keyboard(unsigned char key, int x, int y);
void DoBackgroundStuff();
void specialKeyboardKeys(int key, int x, int y);
void drawBorder(int windowWidth, int windowHeight, int viewportWidth, int viewportHeight);
void updateManLocation(GLfloat x, GLfloat y);
void moveToGridPos(int x, int y);
void read_file(char* argv);
void menu(int choice);
void SetWallTextureOn();
void SetWallTextureOff();
void SetOrthoGroundView();
void SetManGridValues(int x, int y);
void set_up_light(void);
void TurnOnShield();
void TurnOffShield();
void IncreaseCutoffAngle();
void DecreaseCutoffAngle();
void IncreaseFogDensity();
void DecreaseFogDensity();
void MazeInit();
void FogToggle();
void AnimateOverhead();
void EnableTopView();
void MoveLightNorth();
void MoveLightSouth();
void MoveLightEast();
void MoveLightWest();
void MoveLightUp();
void MoveLightDown();
void IncreaseRedLight();
void DecreaseRedLight();
void IncreaseGreenLight();
void DecreaseGreenLight();
void IncreaseBlueLight();
void DecreaseBlueLight();
void IncreaseEmissionLight();
void DecreaseEmissionLight();
void IncreaseDiffuseLight();
void DecreaseDiffuseLight();
void IncreaseAmbiantLight();
void DecreaseAmbiantLight();
void ResetLightPosition();