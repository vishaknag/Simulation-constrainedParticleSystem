#include "render.h"
#include "fog.h"
					
GLuint filter;						
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	
GLuint fogfilter= 1;								// Always use GL_EXP2				
GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};		
double fogExtent = 0.1;

void renderFog()
{
	
	glClearColor(0.5f,0.5f,0.5f,1.0f);				

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);		// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);				// Set Fog Color
	glFogf(GL_FOG_DENSITY, fogExtent);				// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);				// Fog Hint Value
	glFogf(GL_FOG_START, 0.0f);						// Fog Start Depth
	glFogf(GL_FOG_END, 0.5f);						// Fog End Depth
	glEnable(GL_FOG);								// Enables GL_FOG

}
