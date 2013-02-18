/* Source: mouse
 * Description: Contains functions that read mouse input.
 */

#include "render.h"
#include "mouse.h"
#include "vec3d.h"
#include "camera.h"

/* Function getCoord
 * Description: Takes the mouse coordinates in window space and determines its 3D coordinates.
 * Source: http://nehe.gamedev.net/data/articles/article.asp?article=13
 * Input: x - Mouse X-Coordinate
 *        y - Mouse Y-Coordinate
 * Output: newPos - Mouse Coordinates (X, Y, Z) 
 */
point getCoord(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	point newPos;

	// Fetch Window Matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Compute for Mouse Z-Coordinate
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &newPos.x, &newPos.y, &newPos.z);

	return newPos;
} //end getCoord



/* Function: selection
 * Description: Determines if selection of an object on the screen occured. Then save the 
 *              selected object identification into objectName.
 * Input: hits - Number of objects selected.
 *        buffer - Information of the object selected.
 * Output: None
 */

void selection(GLint hits, GLuint *buffer)
{
	int index = 0;// minObj;
	double minDist = DBL_MAX;
	point mouse;
	Vec3d mVec, vertex, vector;

	printf("hits: %d\n", hits);
	if (hits < 1)
	{
		objectName = -1;
	} //end if
	else if (hits == 1)
	{
		index = 0;
		objectName = (int)buffer[index * 4 + 3];
	} //end else if
	else if (hits > 1)
	{
		// Convert mouse coordinates to vector data format
		mouse = getCoord(mousePos.x, mousePos.y);
		mVec = Vec3d(mouse.x, mouse.y, mouse.z);

		// Compute closest vertice to mouse selection coordinate
		for (index = 0; index < hits; index++)
		{
/*			// Search through selection buffer
			minObj = (int)buffer[index * 4 + 3];

			// Determine the distance to mouse selection coordinates
			vertex = Vec3d(objModel->vertices[3*minObj], objModel->vertices[3*minObj + 1], objModel->vertices[3*minObj + 2]);
			vector = vertex.operator - (mVec);
			dist = len(vector);

			// Record closest vertex to mouse selection coordinate
			if (dist < minDist)
			{
				objectName = minObj;
				minDist = dist;
			} //end if
*/
		} //end for
	} //end else if
} //end selection



/* Function: passiveMotion
 * Description: Function that captures and records mouse passive motions
 * Input: x - X-Coordinate of the mouse position
 *        y - Y-Coordinate of the mouse position
 * Output: None
 */
void passiveMotion(int x, int y)
{
	mousePos.x = x;
	mousePos.y = y;
	mousePos.z = 0;
} //end passiveMotion

/* Function: motion
 * Description: Function that captures and records mouse motions
 * Input: x - X-Coordinate of the mouse position
 *        y - Y-Coordinate of the mouse position
 * Output: None
 */
void motion(int x, int y)
{
	if (rightButton)
		mouseRot(x, y);

	mousePos.x = x;
	mousePos.y = y;
} //end motion

/* Function: mouse
 * Description: Function that captures mouse input
 * Input: button - the button pressed on the mouse
 *        state - the state of the button (up/down)
 *        x - X-Coordinate of the mouse position
 *        y - Y-Coordinate of the mouse position
 * Output: None
 */
void mouse(int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];
	GLuint hits = 0;
	GLint viewport[4];

	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
			{
				// Save button state
				leftButton = (state == GLUT_DOWN);

				// Setup Window for Selection mode
				glSelectBuffer(BUFSIZE, selectBuf);
				glGetIntegerv(GL_VIEWPORT, viewport);
				glRenderMode(GL_SELECT);

				// Initialize name stack
				glInitNames();
				glPushName(-1);

				// Setup selection area
				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
					glLoadIdentity();
					gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 1.0, 1.0, viewport);
					gluPerspective(45, ratio, 0.01, 1000);
					glMatrixMode(GL_MODELVIEW);
//				glmDrawPointsSelection(objModel);
				glPopMatrix();
				glFlush();

				// Identify selection area
				hits = glRenderMode(GL_RENDER);
//				selection(hits, selectBuf);

				// Reset windows settings
				glMatrixMode (GL_PROJECTION);
				glLoadIdentity();
				glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
				gluPerspective(45, ratio, 0.01, 1000);

				// Save current mouse position
				//pMousePos.x = x;
				//pMousePos.y = y;
			} //end if
			else if (state == GLUT_UP)
			{
				// Save button state
				leftButton = (state == GLUT_DOWN);
			} //end else
			break;

		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
			{
				// Save button state
				middleButton = (state == GLUT_DOWN);

				// Setup Window for Selection mode
				glSelectBuffer(BUFSIZE, selectBuf);
				glGetIntegerv(GL_VIEWPORT, viewport);
				glRenderMode(GL_SELECT);

				// Initialize name stack
				glInitNames();
				glPushName(-1);

				// Setup selection area
				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
					glLoadIdentity();
					gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 1.0, 1.0, viewport);
					gluPerspective(45, ratio, 0.01, 1000);
					glMatrixMode(GL_MODELVIEW);
					glLoadName(1);
//					glmDraw(objModel, GLM_SMOOTH);
				glPopMatrix();
				glFlush();

				// Identify selection area
				hits = glRenderMode(GL_RENDER);
//				selection(hits, selectBuf);

				// Reset windows settings
				glMatrixMode (GL_PROJECTION);
				glLoadIdentity();
				glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
				gluPerspective(45, ratio, 0.01, 1000);

				// Save current mouse position
				//pMousePos.x = x;
				//pMousePos.y = y;
			} //end if
			else if (state == GLUT_UP)
			{
				// Save button state
				middleButton = (state == GLUT_DOWN);
			} //end else
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
			{
				// Save button state
				rightButton = (state == GLUT_DOWN);
			} //end if
			else if (state == GLUT_UP)
			{
				// Save button state
				rightButton = (state == GLUT_DOWN);
			} //end else
			break;
	} //end switch
} //end mouse

