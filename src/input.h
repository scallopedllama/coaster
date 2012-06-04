#ifndef _INPUT_H_
#define _INPUT_H_

//This header contains the glut callbacks for handling the input events

#ifdef WIN32
// For VC++ you need to include this file as glut.h and gl.h refer to it
#include <windows.h>
// disable the warning for the use of strdup and friends
#pragma warning(disable:4996)
#endif

#include <stdio.h>     // Standard Header For Most Programs
#include <stdlib.h>    // Additional standard Functions (exit() for example)
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

#include <math.h>
#include "util.h"
#include "coaster.h"
#include "gfx.h"

/* State of the mouse */
extern int g_vMousePos[2];
extern int g_iLeftMouseButton;    /* 1 if pressed, 0 if not */
extern int g_iMiddleMouseButton;
extern int g_iRightMouseButton;

/* converts mouse drags into information about rotation/translation/scaling
 * This is run by GL whenever the mouse is moved and a mouse button is being
 * held down.
 */
void mousedrag(int x, int y);

/* This function is called by GL when the mouse moves passively.
 * "Passively" means that no mouse button is being held down */
void mouseidle(int x, int y);

/* This is called by GL whenever a mouse button is pressed. */
void mousebutton(int button, int state, int x, int y);

/* This function will be called by GL whenever a keyboard key is pressed.
 * It recieves the ASCII value of the key that was pressed, along with the x
 * and y coordinates of the mouse at the time the key was pressed.
 */
void keyboard (unsigned char key, int x, int y);

#endif
