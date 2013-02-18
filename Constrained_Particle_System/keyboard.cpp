/* Source: keyboard
 * Description: Contains functions that read keyboard input.
 */

#include "render.h"
#include "keyboard.h"
#include "camera.h"
#include "physics.h"

/* Function: keyboardKeys
 * Description: Reads keyboard input from user.
 * Input: unsigned char key - ASCII value of the key pressed.
 *		  int x - X-position
 *		  int y - Y-position
 * Output: None
 */
void keyboardKeys(unsigned char key, int x, int y) 
{
	switch(key) 
	{
		case 27:
			exit(0);
			break;

		case 'z':
			zoom(-1);
			break;

		case 'x':
			zoom(1);
			break;

		case ' ':
			saveScreenToFile = 1 - saveScreenToFile;
			break;

	} //end switch


} //end keyboardKeys

double angle = 0.0;

/* Function: specialKeys
 * Description: Reads special GLUT keyboard input from user.
 * Input: key - ASCII value of the key pressed.
 *		  int x - X-position
 *		  int y - Y-position
 * Output: None
 */
void specialKeys(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_RIGHT:
			if(PULLX < 1.6)
				PULLX += 0.2;
			break;
 
		case GLUT_KEY_LEFT:
			if(PULLX != 0.0)
				PULLX -= 0.3;
			break;

		case GLUT_KEY_UP:
			if(GRAVITY > 0)
				GRAVITY -= 0.2;
			break;

		case GLUT_KEY_DOWN:
			if(GRAVITY < 6)
				GRAVITY += 0.2;
			break;
	} //end switch
} //end specialKeys