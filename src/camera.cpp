#include "camera.h"

camera::camera(float ma, float d)
{
	//just set all the variables
	dragging = false;
	mouseAdjust = ma;
	theta = 0.000000001;
	psi = M_PI / -2.0;
	distance = d;
	dX = dY = 0.1;
	pos[0] = pos[1] = 0.0;
	pos[2] = 0.5;
	baseLook[0] = baseLook[1] = baseLook[2] = 0.0;
	baseUp = baseLook;
	walkSpeed = .1;
	noClip = true;
	attached = true;
	lighting = false;

	updateCamera();
}

void camera::initLighting()
{
	//sets up a light
	int maxLights=0;
	glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
	if(maxLights > 0)
	{
	    debug("System supports %d lights\n", maxLights);

	    lighting = true;
		glEnable(GL_LIGHT0);
		float color[] = {1.0, 1.0, 1.0};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_SPECULAR, color);
	}
	else
		debug("Camera WARNING: No lights supported on this system.\n");

	updateCamera();
}

//this'll update the actual view information
void camera::updateCamera()
{
	//basically, we're working in spherical space.
	//fyi: spherical to cartesian conversions:
	//x = r*cos(theta)sin(psi)
	//y = r*sin(theta)sin(psi)
	//z = r*cos(psi)

	look[0] = distance*cos(theta)*sin(psi);
	look[1] = distance*sin(theta)*sin(psi);
	look[2] = distance*cos(psi);

	float tp = psi + M_PI / 2.0;
	up[0] = distance*cos(theta)*sin(tp);
	up[1] = distance*sin(theta)*sin(tp);
	up[2] = distance*cos(tp);
}


//tells the camera where to look
void camera::setCamera(CWSpaceVector newPos, CWSpaceVector newLook, CWSpaceVector newUp)
{
	pos = newPos;
	baseLook = newLook;
	baseUp = newUp;
}

//this'll call glulookat
void camera::lookAt()
{
	//set the variables to pass to gluLookAt
	CWSpaceVector camLook = baseLook - look;
	if(!attached)
		camLook = pos - look;
	CWSpaceVector camUp = baseUp;

    if(lighting)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        float lightdir[] = {pos[0], pos[1], pos[2], 1.0};

        glLightfv(GL_LIGHT0, GL_POSITION, lightdir);
    }

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	gluPerspective(45, 1.0 * WINDOW_WIDTH /WINDOW_HEIGHT, 0.01, 1000);
	gluLookAt(pos[0], pos[1], pos[2], camLook[0], camLook[1], camLook[2], camUp[0], camUp[1], camUp[2]);
}

//if b == -2, just mouse motion event
//if b >=0, mouse down
//if b == -1, mouse up
void camera::mouse(int x, int y, int b)
{
	if(b == 0)
	{
		//set dragging true
		dragging = true;
		//and the start x an y
		dX = x;
		dY = y;

		dP = psi;
		dT = theta;

		//done here
		return;
	}

	//make sure it was dragging
	if(b <=0 && dragging)
	{
		//set dragging to false
		if(b == -1)
			dragging = false;

		//find the difference
		int difX = x - dX;
		int difY = y - dY;

		//multiply them by mouseAdjust to get the change in theta/psi
		float deltaT = float(difX) * mouseAdjust;
		float deltaP = float(difY) * mouseAdjust;

		//set theta to deltaT + sT
		theta = dT - deltaT;
		psi = dP + deltaP;

		//update the Camera
		updateCamera();
	}
}

void camera::keyboard(unsigned char key)
{
        switch (key){
        	case 'W':
            case 'w':
            	if(!attached)
            	{
					if(!noClip)
					{
						pos[0] += (walkSpeed * cos(theta));
						pos[1] += (walkSpeed * sin(theta));
					}
					else
					{
						pos[0] -= walkSpeed*cos(theta)*sin(psi);
						pos[1] -= walkSpeed*sin(theta)*sin(psi);
						pos[2] -= walkSpeed*cos(psi);
					}
            	}
                break;
			case 'S':
            case 's':
            	if(!attached)
            	{
					if(!noClip)
					{
						pos[0] -= (walkSpeed * cos(theta));
						pos[1] -= (walkSpeed * sin(theta));
					}
					else
					{
						pos[0] += walkSpeed*cos(theta)*sin(psi);
						pos[1] += walkSpeed*sin(theta)*sin(psi);
						pos[2] += walkSpeed*cos(psi);
					}
            	}
                break;
			case 'A':
            case 'a':
            	if(!attached)
            	{
					pos[0] += (walkSpeed * cos(theta+M_PI/2.0));
					pos[1] += (walkSpeed * sin(theta+M_PI/2.0));
            	}
                break;
			case 'D':
            case 'd':

            	if(!attached)
            	{
					pos[0] += (walkSpeed * cos(theta-M_PI/2.0));
					pos[1] += (walkSpeed * sin(theta-M_PI/2.0));
            	}
                break;
            case ' ':
            	if(!attached)
					pos[2] += walkSpeed;
                break;
			case 'C':
            case 'c':
            	if(!attached)
            		pos[2] -= walkSpeed;
                break;
			case 'N':
            case 'n':
                noClip = !noClip;
                break;
			case 'r':
			case 'R':
            	if(!attached)
            	{
					theta = 0.000000001;
					psi = M_PI / -2.0;
					dX = dY = 0;
					pos[0] = pos[1] = 0;
					pos[2] = 2;
            	}
				break;
			case 'l':
			case 'L':
            	if(!attached)
            		psi = M_PI / -2.0;
				break;
			case 'm':
			case 'M':
				attached = !attached;
				break;
			case 'p':
			case 'P':
				debug("Camera Debug Info:\n");
				debug("\tpos = <%f, %f, %f>, look = <%f, %f, %f>, up = <%f, %f, %f>\n", pos[0], pos[1], pos[2], look[0], look[1], look[2], up[0], up[1], up[2]);
				debug("\t\t\tbaseLook = <%f, %f, %f>, baseUp = <%f, %f, %f>\n", baseLook[0], baseLook[1], baseLook[2], baseUp[0], baseUp[1], baseUp[2]);
		}
        //update the camera
        updateCamera();
}
