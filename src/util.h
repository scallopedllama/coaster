#ifndef _UTIL_H_
#define _UTIL_H_

/* Change these defines to change the size of the window.
 * Any time you deal with the window size you should use these defines.  That
 * way you only have to change the values once.
 * For you final animation (and what you hand in) these should be 640 and 480,
 * but you can change them for testing purposes
 */
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <vector>
#include "cwmtx.h"

using namespace std;
using namespace CwMtx;


//got a little help with the printf wrapper from the following website
// http://bytes.com/topic/c/answers/220856-printf-wrapper
//time stuff help from
// http://www.dreamincode.net/forums/showtopic22330.htm

//some quick wrappers for debug printing.. WILL be expanded in the
//near future.
int debug(char *format, ...);


//dumps the data from a vector<CWSpaceVector> to a file with the specified name
void dumpData(vector<CWSpaceVector> data, char *fname);
void dumpData(vector<float> data, char *fname);


//draws the data on the screen using a line strip, just for debugging. called by drawSpline and drawRawData
void drawData(vector<CWSpaceVector> data);

//draws a quick little thing to show which way is up.
void drawOrigin(float x = 0.0, float y = 0.0, float z = 0.0);

//returns the cross product of the two vectors
CWSpaceVector cross(CWSpaceVector a, CWSpaceVector b);

//returns the dot product of a and b
float dotProduct(CWSpaceVector a, CWSpaceVector b);


//returns the distance between the two points.
float dist(CWSpaceVector v1, CWSpaceVector v2);


//used by draw data and draw points
void drawVertices(vector<CWSpaceVector> data);

//draws the points on screen
void drawPoints(vector <CWSpaceVector> points);

#endif
