#ifndef _GFX_H_
#define _GFX_H_

//This file contains useful functions pertaining GLUT and openGL in general

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

#ifndef M_PI
#define 3.14159265358979323846264338327950288
#endif

#include <OpenGL/gl.h>     // The GL Header File
#include <OpenGL/glu.h>    // The GL Utilities (GLU) header
#include <GLUT/glut.h>   // The GL Utility Toolkit (Glut) Header
#else
#include <GL/gl.h>     // The GL Header File
#include <GL/glu.h>    // The GL Utilities (GLU) header
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
#endif

// Interface to libpicio, provides functions to load/save jpeg files
#include <pic.h>
#include "skybox.h"
#include "coaster.h"

//this indicates whether or not to use displaylists to render the heightmap
extern bool useDL;


/* state of the world */
extern GLfloat g_vLandRotate[3];
extern GLfloat g_vLandTranslate[3];
extern GLfloat g_vLandScale[3];


//initializes glut, creating a window
void initGlut( int &argc, char** argv );

//does initialization of openGL
void initGL();

//called when the window is resized
void handleResize(int w, int h);

//saves a screenshot of the window to the passed filename
void saveScreenshot (char *filename);
void saveScreenshot ();

//loads specified jpeg file into a texture and returns its texture id
GLuint loadTexture(char *filename);

/* This function is called by GL whenever it is idle, usually after calling
* display.
*/
void doIdle();

/* Main GL display loop.
 * This function is called by GL whenever it wants to update the display.
 * All of the drawing you do should be done within this function, or functions
 * that this calls
 */
void display ( void );


#endif
