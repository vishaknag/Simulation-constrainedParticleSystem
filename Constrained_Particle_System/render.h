/* Header: render
 * Description: Header file for rendering graphics in OpenGL
 */

#ifndef _RENDER_H_
#define _RENDER_H_

// Header Files
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "float.h"
#include "openGL-headers.h"
#include "vec3d.h"
#include "glme.h"
#include "glmeEncode.h"
#include "vector.h"


// Mathematics Definitions
#define PI 3.141592653589793238462643383279
#define BUFSIZE 512

// Window size
#define WINRESX 800
#define WINRESY 600

// X, Y, Z
#define X 0
#define Y 1
#define Z 2

// Current working directory
#define GETDIR getcwd

// Colors
#define GOLD 0
#define MAROON 1
#define ORANGE 2

// Themes
extern int THEME1;
extern int THEME2;
extern int THEME3;

// Texture
extern GLuint tiles_id;

// Window settings
extern float ratio;

// File name
extern char filename[50];

// Windows Settings
extern int mainWindowId;

// Light Settings
extern int lighting;
extern int setShineLevel, setSpecLevel, setEmissLevel;
extern int light1, light2, light3, light4, light5, light6, light7, light8; 
extern int materialColor;

// Mouse controls
extern point mousePos, pMousePos;
extern int leftButton, rightButton, middleButton, objectName;

// Camera controls
extern point cameraPos, lineOfSight, viewVector, cameraAng;
extern float cameraRot, cameraDist;
extern double Theta, Phi, R;
extern int pause;

// Constraints
extern int RING;
extern int PIN;
extern double ANIMATE_FACTOR;

// Snapshot
extern int saveScreenToFile;

extern int windowWidth;
extern int windowHeight;
// Bounding Box settings
struct boundBox
{
	struct point norm[6]; // plane normal for each face of the bounding box
	double d[6]; // Distance from the plane to the world origin point
};

extern struct boundBox box;

// Object File Data Structure
// extern GLMmodel *objModel;

//Represents a snap to be texture mapped onto the bounding box wall
class Snap {
	public:
		Snap(char* ps, int w, int h);
		~Snap();

		char* pixelArray;
		int w;
		int h;
};

//Just like auto_ptr, but for arrays
template<class T>
class vishakArray 
{
	private:
		T* array;
		mutable bool isReleased;
	public:
		explicit vishakArray(T* array_ = NULL) :
			array(array_), isReleased(false) {
		}
		
		vishakArray(const vishakArray<T> &aarray) {
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}
		
		~vishakArray() {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
		}
		
		T* get() const {
			return array;
		}
		
		T &operator*() const {
			return *array;
		}
		
		void operator=(const vishakArray<T> &aarray) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}
		
		T* operator->() const {
			return array;
		}
		
		T* release() {
			isReleased = true;
			return array;
		}
	
		T* operator+(int i) {
			return array + i;
		}
		
		T &operator[](int i) {
			return array[i];
		}
};

// fetches the bitmap snap content from the bitmap file
Snap* storeBitmap(const char* fname);

void textureInit();
void drawTextureHome(GLuint tiles_id);

/* Write a screenshot to the specified filename, in PPM format */
void saveScreenshot (int windowWidth, int windowHeight, char *filename);

#endif