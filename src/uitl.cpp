#include "util.h"

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;

int debug(char *format, ...)
{	int toReturn = 0;
	static int callNo = 0;
	printf("%06d: ", callNo);
	callNo++;

	//... option stuff
	va_list ap;
	va_start(ap, format);

	//do the special printf stuff
	toReturn = vprintf(format, ap);

	//clean up the ... stuff
	va_end(ap);

	return toReturn;
}

void dumpData(vector<CWSpaceVector> data, char *out)
{
	debug("Dumping spline data to %s\n", out);
	FILE *o = fopen(out, "w");
	if(o == NULL)
        debug("\tFailed.\n");
    else
        debug("\tOk.\n");

	for(int i=0; i < int(data.size()); i++)
	{
		fprintf(o, "%f %f %f\n", data[i][0], data[i][1], data[i][2]);
	}

	//close that file
	fclose(o);
}

void dumpData(vector<float> data, char *out)
{
	debug("Dumping spline data to %s\n", out);
	FILE *o = fopen(out, "w");
	if(o == NULL)
        debug("\tFailed.\n");
    else
        debug("\tOk.\n");

	for(int i=0; i < int(data.size()); i++)
	{
		fprintf(o, "%f ", data[i]);
	}

	//close that file
	fclose(o);
}

void drawData(vector <CWSpaceVector> points)
{
	glBegin(GL_LINE_STRIP);
		drawVertices(points);
	glEnd();
}

void drawPoints(vector <CWSpaceVector> points)
{
	glBegin(GL_POINTS);
		drawVertices(points);
	glEnd();
}

void drawVertices(vector<CWSpaceVector> data)
{
	int s = data.size();
	for(int i=0; i <= int(data.size()); i++)
	{
		glVertex3f(data[i%s][0], data[i%s][1], data[i%s][2]);
	}
}

void drawOrigin(float x, float y, float z)
{
	glTranslatef(x, y, z);
	glBegin(GL_LINES);
		//+x is red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.5, 0.0, 0.0);
		//-x is dark red
		glColor3f(0.5, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.5, 0.0, 0.0);
		//+y is green
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.5, 0.0);
		//-y is dark green
		glColor3f(0.0, 0.5, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, -0.5, 0.0);
		//+z is blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.5);
		//-z is dark blue
		glColor3f(0.0, 0.0, 0.5);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, -0.5);
	glEnd();
}

CWSpaceVector cross(CWSpaceVector v1, CWSpaceVector v2)
{
	float a = v1[0], b = v1[1], c = v1[2];
	float x = v2[0], y = v2[1], z = v2[2];

	CWSpaceVector r;
	r[0] = b*z - c*y;
	r[1] = c*x - a*z;
	r[2] = a*y - b*x;

	return r;
}

float dotProduct(CWSpaceVector a, CWSpaceVector b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float dist(CWSpaceVector v1, CWSpaceVector v2)
{
	return sqrt( pow(v2[0] - v1[0], 2) + pow(v2[1] - v1[1], 2) + pow(v2[2] - v1[2], 2));
}
