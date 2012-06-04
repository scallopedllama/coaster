/* CSE 418: Project 1 starter code */

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
// Interface to libpicio, provides functions to load/save jpeg files
#include <pic.h>

//my include files
#include "gfx.h"
#include "input.h"
#include "util.h"
#include "skybox.h"
#include "coaster.h"

int g_iMenuId;



/* Function that GL runs once a menu selection has been made.
* This receives the number of the menu item that was selected
*/
void menufunc(int value)
{
	switch (value)
	{
		case 0:
			exit(0);
			break;
		case 1:
			saveScreenshot();
			break;
	}
}

// The ubiquituous main function.
int main ( int argc, char** argv )   // Create Main Function For Bringing It All Together
{
	// get the the filename from the commandline.
	if (argc!=2)
	{
	}


	//initialize glut specific items
	initGlut(argc, argv);

	//initialize openGL specific items
	initGL ();

	// tells glut to use a particular display function to redraw
	glutDisplayFunc(display);

	// allow the user to quit using the right mouse button menu
	// Set menu function callback
	g_iMenuId = glutCreateMenu(menufunc);

	// Set identifier for menu
	glutSetMenu(g_iMenuId);

	// Add quit option to menu
	glutAddMenuEntry("Quit",0);
	glutAddMenuEntry("Take a Snapshot",1);

	// Attach menu to right button clicks
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Set idle function.  You can change this to call code for your animation,
	// or place your animation code in doIdle
	glutIdleFunc(doIdle);

	// callback for keyboard input
	glutKeyboardFunc(keyboard);

	// callback for mouse drags
	glutMotionFunc(mousedrag);

	// callback for idle mouse movement
	glutPassiveMotionFunc(mouseidle);

	// callback for mouse button changes
	glutMouseFunc(mousebutton);

	//initialize global variables
	sky.init();

	c.init("track");

	glutMainLoop        ();          // Initialize The Main Loop

	return 0;
}

