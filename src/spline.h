#ifndef _SPLINE_H_
#define _SPLINE_H_
//this file contains the spline class
//I didn't like the looks of the one provided so I made my own.

#ifdef WIN32
// get rid of ridiculous warnings
#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
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

#define SPLINE_RESOLUTION 0.001
#define SPLINE_SMOOTHNESS 0.5


#include "cwmtx.h"
#include "util.h"
#include <vector>
#include <ctype.h>
#include <string.h>

using namespace std;
using namespace CwMtx;

class spline
{
public:
	//initializes the spline using the file specified.
	//files always start with the number of items followed by a list of
	//points or a list of filenames
	void loadSplineData(char *filename, bool loop = true);

	//renders a line reprsenting the loaded data
	void drawRawData();

	//renders a line reprsenting the whole spline
	void drawSpline();

	//dumps the spline's points
	void dumpSpline();

	//dumps the spline's raw input points
	void dumpRawData();

	CWSpaceVector operator [](unsigned i)
	{
		return points[i];
	}

	inline vector<CWSpaceVector> getData()
	{
		return points;
	}

    //returns the number of points in the interpolated spline
    inline int size()
    {
        return points.size();
    }

    inline CWSpaceVector getTop()
    {
    	return top;
    }

    inline CWSpaceVector getBottom()
    {
    	return bottom;
    }

    inline vector<CWSpaceVector> getRawData()
    {
    	return rawPoints;
    }

    inline void setRawData(vector<CWSpaceVector> data, bool l = true)
    {
    	loop = l;
    	rawPoints = data;
    }

	//uses a catmull-rom spline to find the points inbetween the rawData
	void interpolateData();

private:
	//parses a file full of data points
	int parseDataFile(char *filename, CWSpaceVector &start);

	//this is the higest point in the rollercoaster
	CWSpaceVector top, bottom;

	//this contains the raw data pulled from the files
	vector<CWSpaceVector> rawPoints;
	//this contains all the points of the spline
	vector<CWSpaceVector> points;
	//whether or not to loop the track
	bool loop;

	//opens the specified file and parses the data from it starting all pieces from startX, startY, startZ
	int parseFile(char *filename, CWSpaceVector &start);

	//finds the point at u in between p[1] and p[2], expects p[0-3]
	CWSpaceVector interpolatePoint(float u, CWSpaceVector p[4]);
	//multiplies a 4x4 matrix M and a vector u (expects u[4])
    float* matVecMult4(float u[4], float M[4][4]);
    //multiplies a 3x3 matrix M and a vector v
    //Vec3f matVecMult3(float M[4], Vec3f v);
};

#endif
