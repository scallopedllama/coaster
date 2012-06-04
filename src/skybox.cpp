#include "skybox.h"

skybox sky;

void skybox::loadTextures()
{
	debug("Loading skybox textures\n");

	tId_up = loadTexture("texture/up.jpg");
	tId_down = loadTexture("texture/down.jpg");
	tId_left = loadTexture("texture/left.jpg");
	tId_right = loadTexture("texture/right.jpg");
	tId_front = loadTexture("texture/front.jpg");
	tId_back = loadTexture("texture/back.jpg");
}

void skybox::init()
{
	//load up the textures
	loadTextures();

	//make the display list
	genDisplayList();

	//set up the lighting
	initLighting();
}

void skybox::initLighting()
{
	//sets up a light
	int maxLights=0;
	glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
	if(maxLights > 1)
	{
		glEnable(GL_LIGHT1);
		float color[] = {0.3, 0.3, 0.3};
		glLightfv(GL_LIGHT1, GL_AMBIENT, color);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT1, GL_SPECULAR, color);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        float lightpos[] = {-SKYBOX_SIZE / 5.0, -SKYBOX_SIZE / 2.0, SKYBOX_SIZE / 5.0, 1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
        /*
        float lightdir[] = {camLook[0], camLook[1], camLook[2]};
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir);*/
	}
	else
		debug("Skybox WARNING: No lights supported on this system!\n");
}

void skybox::genDisplayList()
{
	//get a displaylist
	displayListId = glGenLists(1);
	if( glIsList(displayListId) == GL_FALSE )
		debug("Error generating displayList: %s, displayListId is %d\n", gluErrorString(glGetError()), displayListId);

	//render it up
	glNewList(displayListId, GL_COMPILE);
		drawSkybox();
	glEndList();
}

void skybox::render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glCallList(displayListId);
}

//draws the skybox
void skybox::drawSkybox()
{
	GLfloat x1, y1, z1, x2, y2, z2;
	x1 = y1 = z1 = -(SKYBOX_SIZE / 2.0);
	x2 = y2 = z2 = (SKYBOX_SIZE / 2.0);
	//z1 = -4.0;
	//z2 = -4.0 + SKYBOX_SIZE / 6.0;
	GLfloat wiggle = 0.2;

	glEnable(GL_TEXTURE_2D);

	//this could probably be rendered in a more clever way...
	//but this will be good enough.

	//note: render the quads in the following vertex order:
	//			bottom-left, top-left, top-right, bottom-right
	//also note: the vertex coordinates have a "wiggle" value to make the
	//			quads overlap. without this, my system was showing a line
	//			in between them.

	// top
	glBindTexture(GL_TEXTURE_2D, tId_up);
	glBegin(GL_QUADS);
		//texture coords			//vertex coords
		glTexCoord2f(1.0 ,0.0);		glVertex3f(x1 - wiggle, y1 - wiggle, z2);
		glTexCoord2f(0.0 ,0.0);		glVertex3f(x1 - wiggle, y2 + wiggle, z2);
		glTexCoord2f(0.0 ,1.0);		glVertex3f(x2 + wiggle, y2 + wiggle, z2);
		glTexCoord2f(1.0 ,1.0);		glVertex3f(x2 + wiggle, y1 - wiggle, z2);
	glEnd();
	// bottom
	glBindTexture(GL_TEXTURE_2D, tId_down);
	glBegin(GL_QUADS);
		//texture coords			//vertex coords
		glTexCoord2f(1.0 ,0.0);		glVertex3f(x2 + wiggle, y1 - wiggle, z1);
		glTexCoord2f(0.0 ,0.0);		glVertex3f(x2 + wiggle, y2 + wiggle, z1);
		glTexCoord2f(0.0 ,1.0);		glVertex3f(x1 - wiggle, y2 + wiggle, z1);
		glTexCoord2f(1.0 ,1.0);		glVertex3f(x1 - wiggle, y1 - wiggle, z1);
	glEnd();
	// right
	glBindTexture(GL_TEXTURE_2D, tId_right);
	glBegin(GL_QUADS);
		//texture coords			//vertex coords
		glTexCoord2f(1.0 ,0.0);		glVertex3f(x2, y1 - wiggle, z2 + wiggle);
		glTexCoord2f(0.0 ,0.0);		glVertex3f(x2, y2 + wiggle, z2 + wiggle);
		glTexCoord2f(0.0 ,1.0);		glVertex3f(x2, y2 + wiggle, z1 - wiggle);
		glTexCoord2f(1.0 ,1.0);		glVertex3f(x2, y1 - wiggle, z1 - wiggle);
	glEnd();
	// left
	glBindTexture(GL_TEXTURE_2D, tId_left);
	glBegin(GL_QUADS);
		//texture coords			//vertex coords
		glTexCoord2f(0.0 ,1.0);		glVertex3f(x1, y1 - wiggle, z1 - wiggle);
		glTexCoord2f(1.0 ,1.0);		glVertex3f(x1, y2 + wiggle, z1 - wiggle);
		glTexCoord2f(1.0 ,0.0);		glVertex3f(x1, y2 + wiggle, z2 + wiggle);
		glTexCoord2f(0.0 ,0.0);		glVertex3f(x1, y1 - wiggle, z2 + wiggle);
	glEnd();
	//back
	glBindTexture(GL_TEXTURE_2D, tId_back);
	glBegin(GL_QUADS);
		//texture coords			//vertex coords
		glTexCoord2f(0.0 ,0.0);		glVertex3f(x1 - wiggle, y2, z2 + wiggle);
		glTexCoord2f(0.0 ,1.0);		glVertex3f(x1 - wiggle, y2, z1 - wiggle);
		glTexCoord2f(1.0 ,1.0);		glVertex3f(x2 + wiggle, y2, z1 - wiggle);
		glTexCoord2f(1.0 ,0.0);		glVertex3f(x2 + wiggle, y2, z2 + wiggle);
	glEnd();
	// front
	glBindTexture(GL_TEXTURE_2D, tId_front);
	glBegin(GL_QUADS);
		//texture coords			//vertex coords
		glTexCoord2f(1.0 ,1.0);		glVertex3f(x1 - wiggle, y1, z1 - wiggle);
		glTexCoord2f(1.0 ,0.0);		glVertex3f(x1 - wiggle, y1, z2 + wiggle);
		glTexCoord2f(0.0 ,0.0);		glVertex3f(x2 + wiggle, y1, z2 + wiggle);
		glTexCoord2f(0.0 ,1.0);		glVertex3f(x2 + wiggle, y1, z1 - wiggle);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
