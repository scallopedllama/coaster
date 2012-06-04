#ifndef _SKYBOX_H_
#define _SKYBOX_H_

//This file contains the skybox class

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

#include "util.h"	//for debug()
#include "gfx.h"	//for loadTexture()

#define SKYBOX_SIZE 100.0


class skybox
{
public:
	//loads the textures, generates the display list, and inits the lighting
	void init();

	//this will render the skybox on the screen
	void render();

private:

	//this will load up the textures necessary for the skybox
	void loadTextures();

	//this will set up the lighting
	void initLighting();

	//makes the display list
	void genDisplayList();

	//id for the displaylist that renders
	//the whole skybox
	GLuint displayListId;

	//draws the skybox
	void drawSkybox();

	//the skybox texture ids
	GLuint tId_up, tId_down, tId_left, tId_right, tId_front, tId_back;
};

extern skybox sky;

#endif
