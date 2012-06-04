#ifndef _CAMERA_H_
#define _CAMERA_H_

//This header file contains a class that will handle the camera positioning.

#ifdef WIN32
// get rid of ridiculous warnings
#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
#define M_PI 3.14159265358979323846264338327950288
#endif

#ifdef __APPLE__

// This ONLY APPLIES to OSX
//
// Remember to add the -framework OpenGL - framework GLUT to
// to the gcc command line or include those frameworks
// using Xcode

#include <OpenGL/gl.h>     // The GL Header File
#include <OpenGL/glu.h>    // The GL Utilities (GLU) header
#include <GLUT/glut.h>   // The GL Utility Toolkit (Glut) Header
#else
#include <GL/gl.h>     // The GL Header File
#include <GL/glu.h>    // The GL Utilities (GLU) header
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
#endif

#include "cwmtx.h"
#include <math.h>
#include "util.h"

using namespace std;
using namespace CwMtx;

class camera
{
public:
	//need to assign value to mouseAdjust
	camera(float mouseAdjust=0.01, float distance=0.1);

	//takes what keys have been pressed and updates accordingly
	void keyboard(unsigned char c);

	//mouse handler
	//if b == -2, just mouse motion event
	//if b >=0, mouse down
	//if b == -1, mouse up
	void mouse(int x, int y, int b);

	//tells the camera where to look
	void setCamera(CWSpaceVector pos, CWSpaceVector look, CWSpaceVector up);

	//initializes the lighting stuffs.
	void initLighting();

	//this'll call glulookat
	void lookAt();

    //returns whether or not the camera is attached to the track
    inline bool isAttached()
    {
    	return attached;
    }

private:
	//are we dragging?
	bool dragging;

	//can we use lighting
	bool lighting;

	//where the mouse was when dragging started
	int dX, dY;
	float dT, dP;

	//this takes the change in x/y of mouse, multiplies it by this and then
	//adds to psi/theta
	float mouseAdjust;

	//psi and theta
	float psi, theta;

	//how far away from 0,0,0?
	float distance;

	//camera position
	CWSpaceVector pos;
	//a vector representing the direction at which the camera is looking
	CWSpaceVector look;
	//up vector
	CWSpaceVector up;

	//the same as above, these are just the starting point for those two
	CWSpaceVector baseUp, baseLook;

	//the speed at which the camera moves when the wasd keys are pressed
	float walkSpeed;

	//must use space / c to move up / down on z axis?
	//if this is off, you walk in the direction you look
	bool noClip;

	//is the viewer tied to the track?
	bool attached;

	//this'll update the actual view information
	void updateCamera();
};

#endif
