#include "coaster.h"

coaster c;

void coaster::init(char *filename, bool l)
{
    loop = l;
    //camera starts at the beginning
    camTrackPosU = 0.0;

	//set up lighting in the camear
	c.initLighting();

    //load up the spline
    s.loadSplineData(filename, loop);

	//start the camera at that beginning of the track
	camTrackPos = s[0];

	//generate the distances
	genDistances();

    //generate the rails
    genRails();

	//make display list
	genDisplayList();

	//dumpData(rRail, "rRail.dat");
	//dumpData(lRail, "lRail.dat");
	//dumpData(distance, "distance.dat");
}

void coaster::genDisplayList()
{
	//get a displaylist
	displayListId = glGenLists(1);
	if( glIsList(displayListId) == GL_FALSE )
		debug("Error generating displayList: %s, displayListId is %d\n", gluErrorString(glGetError()), displayListId);

	//render it up
	glNewList(displayListId, GL_COMPILE);
		drawSupports();
		drawTrack();
	glEndList();
}

void coaster::render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glCallList(displayListId);
}

void coaster::drawSpline()
{
	glColor3f(0.0, 0.0, 1.0);
    s.drawSpline();
    drawRails();
}

void coaster::drawRails()
{
    glColor3f(0.0, 1.0, 0.0);
    drawData(lRail.getData());
    glColor3f(1.0, 0.0, 0.0);
    drawData(rRail.getData());
}

void coaster::drawTrack()
{
	int s = rRail.size();
	glColor4f(1.0, 1.0, 1.0, 1.0);


	glBegin(GL_QUADS);
		for(int i=0; i<=s; i++)
		{
			CWSpaceVector c = lRail[(i+1)%s] - lRail[i%s];
			CWSpaceVector cr = rRail[i%s] - lRail[i%s];
			CWSpaceVector n0 = cross(c,  cr);
			CWSpaceVector n = lRail[(i+2)%s] - lRail[(i+1)%s];
			CWSpaceVector nr = rRail[(i+1)%s] - lRail[(i+1)%s];
			CWSpaceVector n1 = cross(n, nr);
			n0.makeUnit();
			n1.makeUnit();

			float grey[] = {0.8, 0.8, 0.8};
			glNormal3f(n0[0], n0[1], n0[2]);
			glVertex3f(lRail[i%s][0],		    lRail[i%s][1],		    lRail[i%s][2]);
			glNormal3f(n0[0], n0[1], n0[2]);
			glVertex3f(rRail[i%s][0],		    rRail[i%s][1],		    rRail[i%s][2]);
			glNormal3f(n1[0], n1[1], n1[2]);
			glVertex3f(rRail[(i+1)%s][0],	    rRail[(i+1)%s][1],	    rRail[(i+1)%s][2]);
			glNormal3f(n1[0], n1[1], n1[2]);
			glVertex3f(lRail[(i+1)%s][0],	    lRail[(i+1)%s][1],	    lRail[(i+1)%s][2]);
		}
	glEnd();

	/*
	glBegin(GL_LINES);
		for(int i=0; i<=s; i++)
		{
			glVertex3f(lRail[i%s][0],		    lRail[i%s][1],		    lRail[i%s][2]);
			glVertex3f(rRail[i%s][0],		    rRail[i%s][1],		    rRail[i%s][2]);
		}
	glEnd();

	//draw the normal vectors
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		for(int i=0; i<=s; i++)
		{
			CWSpaceVector c = lRail[(i+1)%s] - lRail[i%s];
			CWSpaceVector cr = rRail[i%s] - lRail[i%s];
			CWSpaceVector n0 = cross(c,  cr);
			CWSpaceVector n = lRail[(i+2)%s] - lRail[(i+1)%s];
			CWSpaceVector nr = rRail[(i+1)%s] - lRail[(i+1)%s];
			CWSpaceVector n1 = cross(n, nr);
			n0.makeUnit();
			n1.makeUnit();

			glVertex3f(lRail[i%s][0],		    lRail[i%s][1],		    lRail[i%s][2]);
			glVertex3f(lRail[i%s][0] + n0[0],   lRail[i%s][1] + n0[1],  lRail[i%s][2] + n0[2]);

			glVertex3f(rRail[(i+1)%s][0],	        rRail[(i+1)%s][1],	        rRail[(i+1)%s][2]);
			glVertex3f(rRail[(i+1)%s][0] + n1[0],   rRail[(i+1)%s][1] + n1[1],  rRail[(i+1)%s][2] + n1[2]);
		}
	glEnd();
*/
	//drawRails();
}

void coaster::drawSupports()
{
    //don't draw a support for every single rail.
	int size = s.size();
	glColor3f(0.8, 0.8, 0.8);

    for(int i=0; i<= 1 / SPLINE_RESOLUTION * 0.8; i++)
    {
        //get the index of the current point we're working on
        int index = int(float(i) * (1 / SPLINE_RESOLUTION * 0.8))% s.size();

        glBegin(GL_TRIANGLE_STRIP);

		    //draw a circular support
		    float steps = 1 / SPLINE_RESOLUTION / 10;
		    for(int j = 0; j <= steps; j++ )
		    {
		        //figure out the points of the circle for this support
		        float t = 2 * M_PI * j / steps;
		        float x = s[index][0] + SUPPORT_RADIUS * cos (t);
		        float y = s[index][1] + SUPPORT_RADIUS * sin (t);

                //find the normal vector
                CWSpaceVector n = CWSpaceVector(x, y, s[index][2]) - s[index];


                //draw the post
                glNormal3f(n[0], n[1], n[2]);
		        glVertex3f(x, y, -40.0);
		        glNormal3f(n[0], n[1], n[2]);
		        glVertex3f(x, y, s[index][2] - 0.015);

		    }

        glEnd();
    }
}

void coaster::genDistances()
{
	int size = s.size();
	//start with 0 at position 0 in the vector
	distance.push_back(0.0);

	for(int i=1; i<=size; i++)
	{
		//find the distance
		float dx = s[(i+1)%size][0] - s[i%size][0];
		float dy = s[(i+1)%size][1] - s[i%size][1];
		float dz = s[(i+1)%size][2] - s[i%size][2];
		float d = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));

		//add that distance to the last distance on the distances
		//vector and push it on
		distance.push_back(distance[i-1] + d);
	}
}

float coaster::getDistance(float u)
{
	//conver that u into an int to get the closest pre-calculated
	//distance index in the distance vector
	int index = int(u);

	//this is the starting distance
	float firstDist = distance[index];

	//find the distance between that point and the next one
	float difDist = distance[index + 1] - firstDist;

	//figure out how much to add to firstDist by multiplying the ratio left
	//of u to the difDist
	//add it all up and return
	float uRat = u - float(index);

	//add it all up and return
	return firstDist + (uRat * difDist);
}

CWSpaceVector coaster::invDistance(float tl, int &in)
{
	int start = 0, end = distance.size();
	int splineLength = s.size();

	//dunno why the f this needs to be here... but
	//on my linux machine, the check in the main loop isn't enough..
	//this is a just in case measure
	if(tl >= distance[end - 1])
		tl -= distance[end - 1];

	int i = 0;
	while (i<10000)
	{
		//guess around the middle
		int index = (start + end) / 2;

		//if it must be in the first half, reset end
		if(distance[index] > tl)
			end = index;
		//if it must be in the second half, reset start
		else if(distance[(index + 1)] < tl)
			start = index;
		//if it's inbetween s[index] and s[index + 1]
		else
		{
			//the position to return is in between index and index + 1
			CWSpaceVector one = s[index];
			//this has to have range checking
			CWSpaceVector two = s[(index + 1) % splineLength];

			//figure out how much is left to go from s[index]
			float remainingDist = tl - distance[index];
			float nextDelta = distance[index + 1] - distance[index];
			float remainingRatio = remainingDist / nextDelta;

			//calculate the return vector
			CWSpaceVector vecRatio = s[(index + 1) % splineLength] - s[index];
			vecRatio = vecRatio * remainingRatio;
			CWSpaceVector r = s[index];
			r = r + vecRatio;

			in = index;
			return r;
		}
		i++;
	}
	debug("WARNING: Did an infinite loop here:\n\ttl %f, start %d, end %d\n", tl, start, end);
	debug("\tmax: %f, distance[nexttolast] %f\n", distance[distance.size() - 1], distance[distance.size() - 2]);
	return CWSpaceVector(0, 0, 0);
}

void coaster::updateCamera()
{
	//if the camera isn't attached, just lookAt
	if(!c.isAttached())
	{
		c.lookAt();
		return;
	}

	//update delta time
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	//it'll take several seconds to get this thing up and running to begin with
	//so this makes sure it'll start at the beginning of the track
	static bool firstRun = true;
	if(firstRun)
	{
		firstRun = false;
		deltaTime = 0;
	}

	//FYI: the last position of the car is stored in the following
	//		variables:
	//			float camTrackPosU;				//u value for \/
	//			CWSpaceVector camTrackPos;		//actual position
	float lastU = camTrackPosU;
	CWSpaceVector lastPos = camTrackPos;

	//get the velocity at that last point
	float v = getVelocity(lastPos[2]);

	//determine how far the car has moved since last update
	float d = v * (deltaTime / 1000.0);
	camTrackPosU = lastU + d;

	//loop the track if necessary
	static int lap = 1;
	float trackLength = distance[distance.size() - 1];
	if(camTrackPosU >= trackLength)
	{
		lap ++;
		debug("\tStarting lap #%d\n", lap);
		camTrackPosU -= trackLength;
	}

	//find the new real-world coordinates of the car
	int index = -1;
	camTrackPos = invDistance(camTrackPosU, index);

	//move the camera up on the z a bit
	CWSpaceVector setLocation = camTrackPos;
	setLocation[2] += CAM_TOP;

	int size = s.size();
	//find the up vector and the look vector for the camera
	//make the camera look several points ahead
	//	find the vector from the car to a point a bit ahead on the track
	//	then add that to the current position of the car to get the look vector
	int toss;
	CWSpaceVector lookAt = invDistance(camTrackPosU + 0.5, toss);
	CWSpaceVector l = lookAt;

	//note: up vector only needs to be messed with if I add banking.
	CWSpaceVector u(0, 0, 1);

	//now that we know where the crap the car is, we can tell
	//the camera where to look
	c.setCamera(setLocation, l, u);

	//tell the camera to look
	c.lookAt();
}

//takes what keys have been pressed and updates the camera accordingly
void coaster::keyboard(unsigned char k)
{
	c.keyboard(k);
}

//handles the mouse movements and clickings
//if b == -2, just mouse motion event
//if b >=0, mouse down
//if b == -1, mouse up
void coaster::mouse(int x, int y, int b)
{
	c.mouse(x, y, b);
}



//finds the angle between this angle and the world (counter-clockwise on z)
float coaster::findWorldTheta(CWSpaceVector p1, CWSpaceVector p2)
{
    //find deltas
    float dx = p2[0] - p1[0];
    float dy = p2[1] - p1[1];
    float dz = p2[2] - p1[2];

    //distance
    float d = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));

    //just in case
    if(d == 0.0)
    {
    	debug("WARNING: distance between points is 0!\n");
    	return 0.0;
    }

	float as = asin(dy / d);
	float ac = acos(dx / d);

	if(as >= 0.0)
			return ac;
	else
			return 2.0 * M_PI - ac;
}


//finds the angle between the p2 - p1 vector and the p3 - p2 vector
float coaster::findTheta(CWSpaceVector p1, CWSpaceVector p2, CWSpaceVector p3)
{
	//get those vectors
	CWSpaceVector v1 = p2 - p1;
	CWSpaceVector v2 = p3 - p2;
	v1[2] = v2[2] = 0.0;

	//find their length
	float lv1 = v1.norm();
	float lv2 = v2.norm();

	//dot product
	float dot = dotProduct(v1, v2);

	//return angle
	return acos(dot / (lv1 * lv2));
}

float coaster::findThetaL(CWSpaceVector p1, CWSpaceVector p2, CWSpaceVector p3)
{
	//get the acute angle between them
	float t = findTheta(p1, p2, p3);

	//return the suppliment
	return M_PI - t;
}

float coaster::findThetaR(CWSpaceVector p1, CWSpaceVector p2, CWSpaceVector p3)
{
	//get the acute angle between them
	float t = findTheta(p1, p2, p3);

	//return the suppliment
	return M_PI + t;
}

//matrices are of form name[row][col]
void coaster::genRails()
{
	debug("Generating rail positions:");
    int points = s.getRawData().size();
    vector<CWSpaceVector> rawLrail, rawRrail;

    //do this for all the points in the spline
    for(int i=0; i<points; i++)
    {
        //get the current point
        CWSpaceVector p1 = s.getRawData()[i];
        CWSpaceVector p2 = s.getRawData()[(i+1)%points];
        CWSpaceVector p3 = s.getRawData()[(i+2)%points];

		//half of that is the angle we want to use
		float xyT = findWorldTheta(p1, p3) + M_PI / 2.0;

		//this works
		float rx = RAIL_DISTANCE * cos(xyT);
		float ry = RAIL_DISTANCE * sin(xyT);
		CWSpaceVector rp2(rx, ry, 0.0);
		CWSpaceVector rp1(-rx, -ry, 0.0);

        //push the translated points onto their respective vectors
        rawLrail.push_back(p2 + rp1);
        rawRrail.push_back(rp2 + p2);
    }
    lRail.setRawData(rawLrail);
    rRail.setRawData(rawRrail);
    lRail.interpolateData();
    rRail.interpolateData();

    printf("done\n");
}
