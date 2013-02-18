/* Header: camera
 * Description: Header file for camera controls
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

// Camera controls
void camInit();
void setCamera();

// Orbital camera
void zoom(int dir);
void mouseRot(int x, int y);

// Free movement camera
void cameraRotateX(int dir);
void cameraRotateY(int dir);
void moveForward(int dir);
void strafe(int dir);
void upDown(int dir);

#endif
