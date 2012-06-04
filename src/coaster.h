#ifndef _COASTER_H_
#define _COASTER_H_

//This file contains the coaster class

#ifdef WIN32
// For VC++ you need to include this file as glut.h and gl.h refer to it
#include <windows.h>
// disable the warning for the use of strdup and friends
#pragma warning(disable:4996)
//for the M_PI that isn't in math on windows for whatever stupid reason
#define M_PI 3.14159265358979323846264338327950288
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

#include <vector>
#include <math.h>
#include "util.h"
#include "cwmtx.h"

#include "spline.h"
#include "camera.h"

#define RAIL_DISTANCE 0.02
//how many screen units represent one meter (units / m)
#define SPACE_CONV 0.05
//defines gravity in meters per second squared
#define GRAVITY (9.8 * SPACE_CONV)
//how close the bisection needs to be in order to count
#define BISECTION_EPSILON 0.1
//this is added to the max z on the velocity function
#define TOP_ADJUST 0.25
//how high off the track the camera sits
#define CAM_TOP 0.1
//the radius of the track supports
#define SUPPORT_RADIUS 0.003


using namespace std;
using namespace CwMtx;

class coaster
{
public:

	//initializes this rollercoaster based off of the passed
	//filename. This function will parse the files, loading up
	//relevant data, generate all the necessary geometry and
	//create the displaylist
	void init(char *filename, bool loop = true);

	//renders the coaster using a displaylist
	void render();

	//draws the spline on screen (may remove later)
    void drawSpline();

    //draws the rails in a line strip
    void drawRails();

	//draws the track using only the lRail and rRail in a gl_quad_strip
	void drawTrack();

	//draws the supports for the track
	void drawSupports();

	inline void drawRawData()
	{
		s.drawRawData();
	}

	//takes what keys have been pressed and updates the camera accordingly
	void keyboard(unsigned char c);

	//handles the mouse movements and clickings
	//if b == -2, just mouse motion event
	//if b >=0, mouse down
	//if b == -1, mouse up
	void mouse(int x, int y, int b);

	//updates the camera position
	void updateCamera();

	//returns whether or not the camera is attached to the track
	inline bool isCameraAttached()
	{
	    return c.isAttached();
	}

private:

    //the camera that will fly along the track
    camera c;

	//finds the rotation about the z axis of the vector pointing
	//from p1 to p2
	float findWorldTheta(CWSpaceVector p1, CWSpaceVector p2);

	//finds the angle between the vector point at p2 from p1 and the vector pointing
	//at p3 from p2
	float findTheta(CWSpaceVector p1, CWSpaceVector p2, CWSpaceVector p3);

	//finds the angle between the vector point at p2 from p1 and the vector pointing
	//at p3 from p2, returns the value of theta for the left side
	float findThetaL(CWSpaceVector p1, CWSpaceVector p2, CWSpaceVector p3);
	float findThetaR(CWSpaceVector p1, CWSpaceVector p2, CWSpaceVector p3);

    //generates points for the rail from the spline data
    void genRails();

	//gets the velocity of the car at the given height in units per s
	inline float getVelocity(float z)
	{
		float adjH = (s.getTop()[2] + TOP_ADJUST) - z ;
		float v = sqrt(2.0 * GRAVITY * adjH);
		return v;
	}

	//converts space units to meters
	inline float toMeters(float units)
	{
		return units / SPACE_CONV;
	}


	//generates the distances vector
	void genDistances();

	//finds the distance along the track between the beginning and point u
	float getDistance(float u);

	//inverse of the distance function, given distance figures out where on track it is
	//returns a position for the coaster to use an sets index to be the index of the
	//point on the spline before the coaster
	//NOTE: does not do any range checking so make sure that the distance is within the appropriate range.
	//		It will only work in the range of distance from the very start all the way back around to the very
	//		start again. second lap will crash it.
	CWSpaceVector invDistance(float s, int &i);

	//id for the displaylist that renders
	//the whole rollercoaster
	GLuint displayListId;

	//generates the display list for the whole coaster
	void genDisplayList();

    //the spline data, may not stay here
    spline s;

    //vector of Vec3fs to store points of left and right rails
    spline rRail;
    spline lRail;

    //whether or not to loop the track
    bool loop;

    //how long since the last update
	int lastTime;

	//this is a vector of distances from the index point to the begnning of the
	//coaster
	vector<float> distance;

	//this is where the camera is on the track in terms of distance
	float camTrackPosU;
	CWSpaceVector camTrackPos;
};

extern coaster c;


#endif
