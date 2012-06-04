#include "gfx.h"

//use a display list to render the heightmap?
bool useDL = true;

void initGlut( int &argc, char** argv )
{
	// Initialize glut
	glutInit(&argc,argv);

	// Set up window modes
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Set window position (where it will appear when it opens)
	glutInitWindowPosition(0,0);

	// Set size of window
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create the window
	glutCreateWindow    ( "CSE 418: Programming Assignment 2" ); // Window Title (argv[0] for current directory as title)

	glutReshapeFunc(handleResize);
}

// initGL will perform the one time initialization by
// setting some state variables that are not going to
// be changed
void initGL ()
{
	// Enable Depth Testing
	glEnable(GL_DEPTH_TEST);
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	//enable blanding
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);

	//enable lighting
	glEnable(GL_LIGHTING);

	// Black Background
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
}

void saveScreenshot()
{
	static int snapshotNo = 0;
	char snapFilename[32];
	sprintf(snapFilename, "snaps/%03d.jpg", snapshotNo);
	saveScreenshot(snapFilename);
	snapshotNo++;
}

/* Write a screenshot to the specified filename */
void saveScreenshot (char *filename)
{
	int i;
	Pic *in = NULL;

	if (filename == NULL)
		return;

	/* Allocate a picture buffer */
	in = pic_alloc(WINDOW_WIDTH, WINDOW_HEIGHT, 3, NULL);

	/* Loop over each row of the image and copy into the image */
	for (i=WINDOW_HEIGHT-1; i>=0; i--)
	{
		glReadPixels(0, WINDOW_HEIGHT-1-i, WINDOW_WIDTH, 1, GL_RGB, GL_UNSIGNED_BYTE, &in->pix[i*in->nx*in->bpp]);
	}

	/* Output the file */
	if (jpeg_write(filename, in))
	{
		debug("File %s saved successfully\n", filename);
	}
	else
	{
		debug("Error saving file %s\n", filename);
	}

	/* Free memory used by image */
	pic_free(in);
}

GLuint loadTexture(char *filename)
{
	GLuint textureId = 0;

	//load the jpeg
	Pic *pJpg = pic_read(filename, NULL);

	if(pJpg == NULL)
	{
		debug("Error loading texture file %s\n", filename);
		exit(1);
	}

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Obtain an id for the texture
	glGenTextures(1, &textureId);

	// Set as the current texture
	glBindTexture(GL_TEXTURE_2D, textureId);

	// set some texture parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

	// Load the texture into OpenGL as a mipmap.
	// NOTE: I may or may not have added support for multiple bitplanes here and it needs to be tested
	//       It's passing the bpp of the jpeg as the number of bytes-per-pixel for the texture. not sure
	//       if that's enough       \right here/
	gluBuild2DMipmaps(GL_TEXTURE_2D, pJpg->bpp, pJpg->nx, pJpg->ny, GL_RGB, GL_UNSIGNED_BYTE, pJpg->pix);

	//turn texturing off
	glDisable(GL_TEXTURE_2D);

	//free the space used for loading the jpeg
	pic_free(pJpg);

	//return the texture id
	return textureId;
}

//based on function in tutorial at
//http://www.dreamincode.net/forums/lofiversion/index.php/t116865.html
//Called when the window is resized
void handleResize(int w, int h)
{
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;

    //map the pixels of the viewport
    glViewport(0, 0, w, h);

    //set the camera to perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	//Reset the camera
	// gluPerspective(camera angle, width-to-height ratio, near z clipping, far z clipping)
    gluPerspective(45.0, (double)w / (double)h, 1.0, 800.0);
}

/* This function is called by GL whenever it is idle, usually after calling
* display.
*/
void doIdle()
{
    if(c.isCameraAttached())
        display();
}

/* Main GL display loop.
 * This function is called by GL whenever it wants to update the display.
 * All of the drawing you do should be done within this function, or functions
 * that this calls
 */
 int lastS = 0, lastTime = 0;
void display ( void )   // Create The Display Function
{
	//update delta time
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	// clear the buffers!
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//render the skybox
	sky.render();

	c.render();

    c.updateCamera();

	// Swap buffers, so one we just drew is displayed
	glutSwapBuffers();

	//show the fps in debug every second or so
	int thisS = currentTime / 5000; /// 33.333;//
	if(thisS != lastS)
	{
		//saveScreenshot();
		debug("%d since last update = %f FPS\n", deltaTime, 1000.0 * pow(float(deltaTime), -1) );
		lastS = thisS;
	}
}
