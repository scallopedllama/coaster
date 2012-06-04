#include "spline.h"

void spline::loadSplineData(char *filename, bool l)
{
	//some of the code in here is based off of the stuff provided from rc_spline.cpp
	loop = l;

	bottom[0] = bottom[1] = top[0] = top[1] = top[2] = 0.0;
	bottom[2] = 999.0;

	CWSpaceVector start;
	start[0] = start[1] = start[2] = 0.0;
	if(parseFile(filename, start) < 0)
	{
		debug("Could not open rollercoaster file \"%s\".", filename);
		exit(1);
	}
	if(rawPoints.size() == 0)
	{
		debug("Got no points for a rollercoaster! exiting.\n");
		exit(1);
	}

	interpolateData();
}

int spline::parseFile(char *filename, CWSpaceVector &start)
{
	debug("Parsing file %s:\n", filename);
	char pwd[1024];
	pwd[0] = '\0';
	if(strchr(filename, '/') != NULL)
	{
		int endCh = 0;
		//this contains a directory so we need to pull that out
		for(int i=int(strlen(filename)); i>=0; i--)
		{
			if(filename[i] == '/')
			{
				endCh = i;
				break;
			}
		}
		strncpy(pwd, filename, endCh);
		pwd[endCh] = '/';
		pwd[endCh + 1] = '\0';
		debug("\tWorking dir: %s\n", pwd);
	}

	//open the current file
	FILE* data = fopen(filename, "r");

	//if the file can't be opened, the function returns -1.
	//this is so that parts of the rollercoaster can simply be
	//ignored if their respective files couldn't be opened.
	//might look funny in the end, but it's better than just crashing.
	if (data == NULL)
	{
		debug ("can't open file %s\n", filename);
		return -1;
	}

	int pieces;

	// get the number of pieces for this file
	fscanf(data, "%d", &pieces);

	//and the rest
	for (int j = 0; j < pieces; j++)
	{
		// define a variable of a reasonable size for a filename
		char newFilename[1024];
		fscanf(data, "%s", newFilename);

		if(newFilename[0] == '#')
		{
			debug("\tIgnoring comment\n");
			continue;
		}

		char toOpen[1024];
		strcpy(toOpen, pwd);
		strcat(toOpen, newFilename);
		parseDataFile(toOpen, start);
	}
	return 0;
}


int spline::parseDataFile(char *filename, CWSpaceVector &start)
{
	//open the current file
	FILE* data = fopen(filename, "r");

	//if the file can't be opened, the function returns -1.
	//this is so that parts of the rollercoaster can simply be
	//ignored if their respective files couldn't be opened.
	//might look funny in the end, but it's better than just crashing.
	if (data == NULL)
	{
		debug ("can't open file %s\n", filename);
		return -1;
	}

	int pieces;

	// get the number of pieces for this file
	fscanf(data, "%d", &pieces);
	//it's a list of points to parse
	CWSpaceVector point;
	float x, y, z;

	//now scan the rest of the file
	while (fscanf(data, "%f %f %f", &x, &y, &z) != EOF)
	{
		point[0] = x + start[0];
		point[1] = y + start[1];
		point[2] = z + start[2];

		rawPoints.push_back(point);
	}


	// now close the file
	fclose(data);

	//reset the start values to be after the last part of this
	//piece
	if(rawPoints.size() > 0)
	{
		start[0] = rawPoints[rawPoints.size() - 1][0];
		start[1] = rawPoints[rawPoints.size() - 1][1];
		start[2] = rawPoints[rawPoints.size() - 1][2];

	}
	return 0;
}

void spline::drawRawData()
{
	drawData(rawPoints);
}

void spline::drawSpline()
{
	drawData(points);
}

void spline::dumpRawData()
{
    dumpData(rawPoints, "raw_data.dat");
}

void spline::dumpSpline()
{
    dumpData(points, "spline.dat");
}

void spline::interpolateData()
{
	debug("Interpolating spline data: ");
    int s = rawPoints.size();
	int e = s - 1;

	//if we're looping, need to do
	if(loop)
		e = s + 2;

    for(int i=2; i<e; i++)
    {
        //put the start point on
        points.push_back(rawPoints[(i - 1)%s]);

        //see if that was the highest point
        if(rawPoints[(i-1)%s][2] > top[2])
        	top = rawPoints[(i-2)%s];

        //interpolate all the points in between
        for(float u = SPLINE_RESOLUTION; u <= 1.0 - SPLINE_RESOLUTION; u += SPLINE_RESOLUTION)
        {
            //load up the point vector, use modulus division to prevent
            //an out of bounds.
            CWSpaceVector p[4];
            p[0] = rawPoints[(i - 2) % s];
            p[1] = rawPoints[(i - 1) % s];
            p[2] = rawPoints[i % s];
            p[3] = rawPoints[(i + 1) % s];

            //interpolate it up!
            CWSpaceVector c = interpolatePoint(u, p);

			//see if it's the hightest yet
			if(c[2] > top[2])
				top = c;
			if(c[2] < bottom[2])
				bottom = c;

            //push that new point on the points vector
            points.push_back(c);

        }
    }
    printf("done\n");
}

CWSpaceVector spline::interpolatePoint(float u_val, CWSpaceVector p[4])
{
	//set up that matrix
	CWTMatrix<float> M(4, 4);
	M[0][0] = 0.0;					M[0][1] = 1.0;					M[0][2] = 0.0;						M[0][3] = 0.0;
	M[1][0] = -SPLINE_SMOOTHNESS;	M[1][1] = 0.0;					M[1][2] =SPLINE_SMOOTHNESS;			M[1][3] = 0.0;
	M[2][0] = 2*SPLINE_SMOOTHNESS;	M[2][1] = SPLINE_SMOOTHNESS-3;	M[2][2] = 3-2*SPLINE_SMOOTHNESS;	M[2][3] = -SPLINE_SMOOTHNESS;
	M[3][0] = -SPLINE_SMOOTHNESS;	M[3][1] = 2-SPLINE_SMOOTHNESS;	M[3][2] = SPLINE_SMOOTHNESS-2;		M[3][3] = SPLINE_SMOOTHNESS;

	//set up u vector
	CWTMatrix<float> u(1, 4);
	u[0][0] = 1.0;		u[0][1] = u_val;		u[0][2] = u_val * u_val;	u[0][3] = u_val * u_val * u_val;

	//set up points matrix
	CWTMatrix<float> P(4, 4);
	for(int r=0; r<4; r++)
	{
        P[r][0] = p[r][0];
        P[r][1] = p[r][1];
        P[r][2] = p[r][2];
        P[r][3] = 0.0;
	}

	CWTMatrix<float> uM(1, 4);
	CWTMatrix<float> r(1, 4);

	//do the math
	uM = u * M;
	r = uM * P;

	//return the found point
	CWSpaceVector ret(r[0][0], r[0][1], r[0][2]);

	return ret;
}
