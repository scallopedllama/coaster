#include "input.h"


/* converts mouse drags into information about rotation/translation/scaling
 * This is run by GL whenever the mouse is moved and a mouse button is being
 * held down.
 */
void mousedrag(int x, int y)
{
    c.mouse(x, y, -2);
	display();
}


/* This function is called by GL when the mouse moves passively.
* "Passively" means that no mouse button is being held down */
void mouseidle(int x, int y)
{
    c.mouse(x, y, -2);
	display();
}



/* This is called by GL whenever a mouse button is pressed. */
void mousebutton(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
        c.mouse(x, y, button);
    if(state == GLUT_UP)
        c.mouse(x, y, -1);
	display();
}

/* This function will be called by GL whenever a keyboard key is pressed.
* It recieves the ASCII value of the key that was pressed, along with the x
* and y coordinates of the mouse at the time the key was pressed.
*/
void keyboard (unsigned char key, int x, int y) {

	// User pressed quit key
	if(key == 'q' || key == 'Q' || key == 27) {
		exit(0);
	}

    c.keyboard(key);
	display();
}
