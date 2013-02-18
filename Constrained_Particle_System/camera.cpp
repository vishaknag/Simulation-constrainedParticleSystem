/* Source: camera
 * Description: Contains functions that controls the camera.
 */

#include "render.h"
#include "camera.h"
#include "vec3d.h"

/* Function: camInit
 * Description: Initializes the camera control variables.
 * Input: None
 * Output: None
 */
void camInit()
{
	Theta = 0;  // Z-X plane
	Phi = PI + PI/2;  // Y-X plane
	R = 5.5;

	cameraPos.x = R * cos(Phi) * cos (Theta);
	cameraPos.y = R * sin (Theta);
	cameraPos.z = -R * sin(Phi) * cos (Theta);

	lineOfSight.x = 0.0;
	lineOfSight.y = -0.7;
	lineOfSight.z = 0.0;

	viewVector.x = 0.0;
	viewVector.y = 1.0;
	viewVector.z = 0.0;

} //end camInit


/* Function: setCamera()
 * Description: Sets the camera position and viewing.
 * Input: None
 * Output: None
 */
void setCamera()
{

	glLoadIdentity();
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, lineOfSight.x, lineOfSight.y, lineOfSight.z, viewVector.x, viewVector.y, viewVector.z);

} //end setCamera


/* Function: zoom
 * Description: Zooms the camera in and out.
 * Input: dir - Zoom direction.  (Zoom In = 1, Zoom Out = -1)
 * Output: None
 */
void zoom(int dir)
{
	R += dir*0.1; // Increment/decrement zoom distance

	if (R < 0.2)  // Zoom In Limit
		R = 0.2;

	// Recompute camera position
	cameraPos.x = R * cos(Phi) * cos (Theta);
	cameraPos.y = R * sin (Theta);
	cameraPos.z = -R * sin(Phi) * cos (Theta);
	setCamera();
} //end zoom


/* Function: mouseRot
 * Description: Rotates the entire camera about a point using the mouse.
 * Input: x - X Coordinate of the mouse
 *        y - Y Coordinate of the mouse
 * Output: None
 */
void mouseRot(int x, int y)
{
	point mouse;
	Vec3d cameraPoint, originPoint;

	mouse.x = x - mousePos.x;
	mouse.y = y - mousePos.y;

	Phi += mouse.x * 0.01;
	Theta += mouse.y * 0.01;
    
    if (Phi > (2 * PI))
		Phi -= 2 * PI;
    
    if (Phi < 0)
		Phi += 2 * PI;
    
    if (Theta > (PI / 2 - 0.01)) // dont let the point enter the north pole
		Theta = PI / 2 - 0.01;
    
    if (Theta < (-PI / 2 + 0.01))
		Theta = -PI / 2 + 0.01;

	// Recompute camera position
	cameraPos.x = R * cos(Phi) * cos (Theta);
	cameraPos.y = R * sin (Theta);
	cameraPos.z = -R * sin(Phi) * cos (Theta);

	setCamera();

} //end mouseRot

/* Function: cameraRotateX
 * Description: Rotates the entire camera on its x-axis.
 * Input: dir - Direction of the angle cameraRotation (Right = 1, Left = -1)
 * Output: None
 */
void cameraRotateX(int dir)
{
	cameraAng.x += dir*cameraRot;
	lineOfSight.y = -sin(cameraAng.x);
	lineOfSight.z = -cos(cameraAng.x);
	setCamera();
} // end cameraRotateX

/* Function: cameraRotateY
 * Description: Rotates the entire camera on its y-axis.
 * Input: dir - Direction of the angle cameraRotation (Look Up = -1, Look Down = 1)
 * Output: None
 */
void cameraRotateY(int dir)
{
	cameraAng.y += dir*cameraRot;
	lineOfSight.x = sin(cameraAng.y);
	lineOfSight.z = -cos(cameraAng.y);
	setCamera();
} //end cameraRotateY

/* Function: moveForward
 * Description: Moves the entire camera forward or backwards.
 * Input: dir - Direction of the movement (Forward = 1, Backward = -1)
 * Output: None
 */
void moveForward(int dir) 
{
	cameraPos.x += dir*cameraDist;
	cameraPos.y -= dir*cameraDist;
	cameraPos.z += dir*cameraDist;
	lineOfSight.x += dir*cameraDist;
	lineOfSight.y -= dir*cameraDist;
	lineOfSight.z += dir*cameraDist;
	setCamera();
} //end moveForward

/* Function: strafe
 * Description: Strafes the entire camera left or right.
 * Input: dir - Direction of the movement (Right = 1, Left = -1)
 * Output: None
 */
void strafe(int dir) 
{
	cameraPos.x -= dir*(lineOfSight.z)*cameraDist;
	cameraPos.z += dir*(lineOfSight.x)*cameraDist;
	//lineOfSight.x -= cameraPos.x;
	//lineOfSight.y += cameraPos.y;
	//lineOfSight.z += cameraPos.z;
	setCamera();
} //end strafe

/* Function: upDown
 * Description: Moves the entire camera vertically up or down.
 * Input: dir - Direction of the movement (Up = 1, Down = -1)
 * Output: None
 */
void upDown(int dir) 
{
	cameraPos.y -= dir*(lineOfSight.z)*cameraDist;
	//lineOfSight.x += cameraPos.x;
	//lineOfSight.y += cameraPos.y;
	//lineOfSight.z += cameraPos.z;
	setCamera();
} //end upDown