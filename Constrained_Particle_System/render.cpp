/* Source: render
 * Description: Contains functions that render graphics using OpenGL.
 */

#include "render.h"
#include "lights.h"
#include "mouse.h"
#include "keyboard.h"
#include "camera.h"
#include "world.h"
#include "physics.h"
#include "matrix.h"
#include "chain.h"
#include "fog.h"
#include "pic.h"

#define PARTICLES 1
#define SIMULATION 2
#define RINGS 3
#define UNDERWATER_RADIO 4
#define WALLS 5
#define RINGCONSTRAINT 6
#define FOG 7
#define TSTEP 8
#define VDAMPING 9
#define GRAVITYFORCE 10
#define BETAFACTOR 11
#define TESTCASE 12

int pauseSimulation;
int flag = 0;
int count = 0;

/* Global Variables BEGIN */
// Window settings
float ratio;
int mainWindowId = 0;

// File name
char filename[50];

// Light controls
int lighting;
int setShineLevel, setSpecLevel, setEmissLevel;
int light1, light2, light3, light4, light5, light6, light7, light8;
int materialColor;

// Mouse controls
point mousePos, pMousePos;
int leftButton, rightButton, middleButton, objectName = -1;

// Camera controls
point cameraPos, lineOfSight, viewVector, cameraAng;
float cameraRot, cameraDist;

// camera parameters
double Theta, Phi, R;

int pause = 0;
int ringValue = 0;
int *ringChecked = &ringValue;
int particleValue = 2;
int *particleRadioChecked = &particleValue;
int underwaterValue = 0;
int *underwaterChecked = &underwaterValue;
double ANIMATE_FACTOR = 0.1;
int fogValue = 0;
int *fogRadioChecked = &fogValue;
float tStep = 0.003;
float *Timestep = &tStep;
float vDampValue = 0.1;
float *VelocityDamping = &vDampValue;
float gravityValue = 1;
float *gravityPull = &gravityValue;
float betaValue = 0.9;
float *betaFactor = &betaValue;

// Themes
int THEME1 = 1;
int THEME2 = 0;
int THEME3 = 0;

// Constraints
int RING = 1;
int PIN = 1;

GLMmodel *objModel;
GLUquadricObj* sphere;

// Snap shot
int windowWidth = 800;
int windowHeight = 700;

// number of images saved to disk so far
int sprite=0, mycount = 0;
int saveScreenToFile=0;

/* Global Variables END */

/* Write a screenshot, in the PPM format, to the specified filename, in PPM format */
void saveScreenshot(int windowWidth, int windowHeight, char *filename)
{
  if (filename == NULL)
    return;

  // Allocate a picture buffer 
  Pic * in = pic_alloc(windowWidth, windowHeight, 3, NULL);

  printf("File to save to: %s\n", filename);

  for (int i=windowHeight-1; i>=0; i--) 
  {
    glReadPixels(0, windowHeight-i-1, windowWidth, 1, GL_RGB, GL_UNSIGNED_BYTE,
      &in->pix[i*in->nx*in->bpp]);
  }

  if (ppm_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");

  pic_free(in);
}


/* Function: initialize
 * Description: Initializes all variables and settings for the application.
 * Input: None
 * Output: None
 */
void initialize()
{
	// Initialize all variables
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 0.01, 1000);

	// Set Background Color
//	glClearColor (0.87, 0.58, 0.98, 1.0);		// LIGHT PURPLE
//	glClearColor (0.74902, 0.847059, 0.847059, 1.0);  // LIGHT BLUE
	glClearColor(0.8, 0.8, 0.8, 1.0);

	glEnable(GL_DEPTH_TEST);

	// Initialize Light
	lightInit();

	// Initialize Camera
	camInit();

	// Initialize Environment
	worldInit();

} //end initialize


void control_cb(int control)
{
	if(control == PARTICLES)
	{
		if(*particleRadioChecked == 0)
		{
			PCOUNT = 15;
			GRAVITY = 1.0;
			ParticleSystemInit();
			ANIMATE_FACTOR = 0.1;
			RING = 1;
		}
		if(*particleRadioChecked == 1)
		{
			PCOUNT = 13;
			GRAVITY = 1.0;
			ParticleSystemInit();
			ANIMATE_FACTOR = 0.09;
			RING = 1;
		}
		if(*particleRadioChecked == 2)
		{
			PCOUNT = 11;
			GRAVITY = 1.0;
			ParticleSystemInit();
			ANIMATE_FACTOR = 0.08;
			RING = 1;
		}
		if(*particleRadioChecked == 3)
		{
			PCOUNT = 9;
			GRAVITY = 1.0;
			ParticleSystemInit();
			ANIMATE_FACTOR = 0.07;
			RING = 1;
		}
		if(*particleRadioChecked == 4)
		{
			PCOUNT = 7;
			GRAVITY = 1.0;
			ParticleSystemInit();
			ANIMATE_FACTOR = 0.06;
			RING = 1;
		}
		if(*particleRadioChecked == 5)
		{
			PCOUNT = 5;
			GRAVITY = 1.0;
			ParticleSystemInit();
			ANIMATE_FACTOR = 0.05;
			RING = 1;
		}
		if(*particleRadioChecked == 6)
		{
			PCOUNT = 3;
			ParticleSystemInit();
			GRAVITY = 1.0;
			ANIMATE_FACTOR = 0.04;
			RING = 1;
		}
	}

	if(control == SIMULATION)
	{
		pauseSimulation = 1 - pauseSimulation;
	}

	if(control == RINGS)
	{
		if(*ringChecked == 0)
		{
			pRINGX = 0.0;
			pRINGY = -0.5;
			GRAVITY = 1.0;
			ParticleSystemInit();
		}
		if(*ringChecked == 1)
		{
			pRINGX = 0.0;
			pRINGY = -0.75;
			GRAVITY = 1.0;
			ParticleSystemInit();
		}
		if(*ringChecked == 2)
		{
			pRINGX = 0.0;
			pRINGY = -1.0;
			GRAVITY = 1.0;
			ParticleSystemInit();
		}
	}

	if(control == UNDERWATER_RADIO)
	{
		if(*underwaterChecked == 0)
		{
			UNDERWATER = 1;
		}
		if(*underwaterChecked == 1)
		{
			UNDERWATER = 0;
		}
	}

	if(control == FOG)
	{
		if(*fogRadioChecked == 0)
		{
			fogExtent = 0.1;
		}

		if(*fogRadioChecked == 1)
		{
			fogExtent = 0.15;
		}

		if(*fogRadioChecked == 2)
		{
			fogExtent = 0.20;
		}
	}

	if(control == TSTEP)
	{
		pSystemObj->h = *Timestep;
		TIMESTEP = *Timestep;
		GRAVITY = 1.0;
		ParticleSystemInit();
	}

	if(control == VDAMPING)
	{
		UNDERWATER_ALPHA = *VelocityDamping;
	}

	if(control == GRAVITYFORCE)
	{
		GRAVITY = *gravityPull;
	}
	
	if(control == WALLS)
	{
		Animate = 1 - Animate;
	}

	if(control == RINGCONSTRAINT)
	{
		RING = 1 - RING;
	}

	if(control == BETAFACTOR)
	{
		BETA = *betaFactor;
	}

	if(control == TESTCASE)
	{
		RUNTEST = 1 - RUNTEST;
		RING = 0;
		GRAVITY = 1.0;
		ParticleSystemInit();
	}
}


/* Function: createGLUI
 * Description: Creates and initializes the GLUI window.
 * Input: None
 * Output: None
 */
void createGLUI()
{
	// Create a GLUI window
	GLUI *glui = GLUI_Master.create_glui("controls", 0, 600, -1);
	//GLUI *glui = GLUI_Master.create_glui_subwindow( mainWindowId, GLUI_SUBWINDOW_BOTTOM ); 

    glui->set_main_gfx_window( mainWindowId );

	GLUI_Panel *main_panel = glui->add_panel ( "Constrained Particle System" );

		GLUI_Panel *lights_panel = glui->add_panel_to_panel( main_panel, "LIGHTS", 1 );

			glui->add_statictext_to_panel( lights_panel, "ON / OFF" );
			glui->add_checkbox_to_panel( lights_panel, "Light 1", &light1 );
			glui->add_checkbox_to_panel( lights_panel, "Light 2", &light2 );
			glui->add_checkbox_to_panel( lights_panel, "Light 3", &light3 );
			glui->add_checkbox_to_panel( lights_panel, "Light 4", &light4 );
			glui->add_checkbox_to_panel( lights_panel, "Light 5", &light5 );
			glui->add_checkbox_to_panel( lights_panel, "Light 6", &light6 );
			glui->add_checkbox_to_panel( lights_panel, "Light 7", &light7 );
			glui->add_checkbox_to_panel( lights_panel, "Light 8", &light8 );

			glui->add_column_to_panel(lights_panel, true);

			glui->add_statictext_to_panel( lights_panel, "EFFECTS" );
			glui->add_checkbox_to_panel( lights_panel, "Shininess", &setShineLevel );
			glui->add_checkbox_to_panel( lights_panel, "Specularity", &setSpecLevel );
			glui->add_checkbox_to_panel( lights_panel, "Emissivity", &setEmissLevel );
			glui->add_separator_to_panel(lights_panel);

			glui->add_statictext_to_panel( lights_panel, "FOG" );
			GLUI_RadioGroup *fogLevel = glui->add_radiogroup_to_panel(lights_panel, fogRadioChecked, FOG, control_cb);
			glui->add_radiobutton_to_group( fogLevel, "Low" );
			glui->add_radiobutton_to_group( fogLevel, "Medium" );
			glui->add_radiobutton_to_group( fogLevel, "High" );
			
			GLUI_Panel *Dynamic_panel = glui->add_panel_to_panel( main_panel, "DYNAMIC", 1);

			GLUI_Panel *particles_panel = glui->add_panel_to_panel( Dynamic_panel, "PARTICLES", 1 );

			// Initialize the Physics module
			GLUI_RadioGroup *particleCount = glui->add_radiogroup_to_panel(particles_panel, particleRadioChecked, PARTICLES, control_cb);
			glui->add_radiobutton_to_group( particleCount, "15" );
			glui->add_radiobutton_to_group( particleCount, "13" );
			glui->add_radiobutton_to_group( particleCount, "11" );
			glui->add_radiobutton_to_group( particleCount, "9" );
			glui->add_radiobutton_to_group( particleCount, "7" );
			glui->add_radiobutton_to_group( particleCount, "5" );
			glui->add_radiobutton_to_group( particleCount, "3" );

			glui->add_column_to_panel(Dynamic_panel, true);
			
			GLUI_Panel *animate_panel = glui->add_panel_to_panel( Dynamic_panel, "ANIMATE WALLS", 1 );
			GLUI_Button *animateButton = glui->add_button_to_panel( animate_panel, "ANIMATE", WALLS, control_cb);
			
			glui->add_separator_to_panel(Dynamic_panel);

			GLUI_Panel *simulation_panel = glui->add_panel_to_panel( Dynamic_panel, "SIMULATION", 1 );
			GLUI_Button *pauseButton = glui->add_button_to_panel( simulation_panel, "PAUSE", SIMULATION, control_cb);

			glui->add_separator_to_panel(Dynamic_panel);

			GLUI_Panel *constraint_panel = glui->add_panel_to_panel( Dynamic_panel, "CONSTRAINT", 1 );
			GLUI_Button *ringConstraintButton = glui->add_button_to_panel( constraint_panel, "RING", RINGCONSTRAINT, control_cb);

			GLUI_Panel *testcase_panel = glui->add_panel_to_panel( Dynamic_panel, "TESTCASE", 1 );
			GLUI_Button *runTestcaseButton = glui->add_button_to_panel( testcase_panel, "RUN TEST", TESTCASE, control_cb);

			glui->add_column_to_panel(main_panel, true);

			GLUI_Panel *ring_panel = glui->add_panel_to_panel( main_panel, "RINGS", 1 );
			GLUI_RadioGroup *ringSelection = glui->add_radiogroup_to_panel(ring_panel, ringChecked, RINGS, control_cb);
			glui->add_radiobutton_to_group( ringSelection, "INNER RING" );
			glui->add_radiobutton_to_group( ringSelection, "MIDDLE RING" );
			glui->add_radiobutton_to_group( ringSelection, "OUTER RING" );
			
			glui->add_separator_to_panel(ring_panel);
			glui->add_statictext_to_panel(ring_panel, "RING COLORS" );
			GLUI_RadioGroup *matColor =	glui->add_radiogroup_to_panel(ring_panel, &materialColor);
			glui->add_radiobutton_to_group( matColor, "Gold" );
			glui->add_radiobutton_to_group( matColor, "Maroon" );
			glui->add_radiobutton_to_group( matColor, "Orange" );

			GLUI_Panel *underwater_panel = glui->add_panel_to_panel( main_panel, "DAMPING", 1 );
			GLUI_RadioGroup *dampingSelection = glui->add_radiogroup_to_panel(underwater_panel, underwaterChecked, UNDERWATER_RADIO, control_cb);
			glui->add_radiobutton_to_group( dampingSelection, "UNDERWATER" );
			glui->add_radiobutton_to_group( dampingSelection, "ABOVE WATER" );

			GLUI_Panel *parameters_panel = glui->add_panel_to_panel( main_panel, "PARAMETERS", 1 );
			GLUI_EditText *tStepParam = glui->add_edittext_to_panel( parameters_panel, "TSTEP", GLUI_EDITTEXT_FLOAT, Timestep, TSTEP, control_cb);
			GLUI_EditText *vDampParam = glui->add_edittext_to_panel( parameters_panel, "VDAMP", GLUI_EDITTEXT_FLOAT, VelocityDamping, VDAMPING, control_cb);
			GLUI_EditText *gravityParam = glui->add_edittext_to_panel( parameters_panel, "GRAVITY -", GLUI_EDITTEXT_FLOAT, gravityPull, GRAVITYFORCE, control_cb);
			GLUI_EditText *betaParam = glui->add_edittext_to_panel( parameters_panel, "BETA", GLUI_EDITTEXT_FLOAT, betaFactor, BETAFACTOR, control_cb);

		//	GLUI_Panel *constraintVector_panel = glui->add_panel_to_panel( main_panel, "CONSTRAINT VECTOR", 1 );
		//	GLUI_EditText *tStepParam = glui->add_edittext_to_panel( parameters_panel, "TSTEP", GLUI_EDITTEXT_FLOAT, Timestep, TSTEP, control_cb);
			
	glui->set_main_gfx_window( mainWindowId );
} //end createGLUI

/* Function: display
 * Description: Renders all specified graphics on the given window.
 * Input: None
 * Output: None
 */
void display(void)
{
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// Setup camera position;
	setCamera();

	// Setup lights
	setLights();
	enableLights();

	renderAxis();

	renderBoundingBox();
	
	DrawPSystem();

	renderFog();

	drawTextureHome(tiles_id);

	glutSwapBuffers();
} //end display

/* Function: idle
 * Description: Continues rendering all specified graphics while no inputs are given.
 * Input: None
 * Output: None
 */
void idle(void)
{
	/* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
    if ( glutGetWindow() != mainWindowId ) 
		glutSetWindow(mainWindowId); 

  char s[20]="picxxxx.ppm";
  
  // save screen to file
  s[3] = 48 + (sprite / 1000);
  s[4] = 48 + (sprite % 1000) / 100;
  s[5] = 48 + (sprite % 100 ) / 10;
  s[6] = 48 + sprite % 10;

  mycount++;

	// Snapshots
	if (saveScreenToFile==1 && mycount%4 == 0)
	  {
		saveScreenshot(windowWidth, windowHeight, s);
	    //saveScreenToFile=0; // save only once, change this if you want continuos image generation (i.e. animation)
		sprite++;
	  }

	  if (sprite >= 800) // allow only 300 snapshots
	  {
		exit(0);	
	  }


	// insert code which appropriately performs one step of the cube simulation:
	if(pauseSimulation == 1)
	{
		Euler();
		  count++;
		  if(Animate)
			{
				if(count > 10)
				{
					count = 0;
				}

				if(flag == 0)
				{
					if(count == 1)
					{
						tallY += -ANIMATE_FACTOR;
						shortY -= -ANIMATE_FACTOR;
					}
					if(count == 2)
					{
						tallY += -ANIMATE_FACTOR;
						shortY -= -ANIMATE_FACTOR;
					}
					if(count == 3)
					{
						tallY += -ANIMATE_FACTOR;
						shortY -= -ANIMATE_FACTOR;
					}
					if(count == 4)
					{
						tallY += -ANIMATE_FACTOR;
						shortY -= -ANIMATE_FACTOR;
					}
					if(count == 5)
					{
						tallY += -ANIMATE_FACTOR;
						shortY -= -ANIMATE_FACTOR;
						flag = 1;
					}
				}
				else if(flag == 1)
				{

					if(count == 6)
					{
						tallY -= -ANIMATE_FACTOR;
						shortY += -ANIMATE_FACTOR;
					}
					if(count == 7)
					{
						tallY -= -ANIMATE_FACTOR;
						shortY += -ANIMATE_FACTOR;
					}
					if(count == 8)
					{
						tallY -= -ANIMATE_FACTOR;
						shortY += -ANIMATE_FACTOR;
					}
					if(count == 9)
					{
						tallY -= -ANIMATE_FACTOR;
						shortY += -ANIMATE_FACTOR;
					}
					if(count == 10)
					{
						tallY -= -ANIMATE_FACTOR;
						shortY += -ANIMATE_FACTOR;
						flag = 0;
					}
				}
			}

		}
	glutPostRedisplay();
} //end idle


/* Function: reshape
 * Description: Redraws the window when the user changes the window size.
 * Input: w - width of the window
 *        h - height of the window
 * Output: None
 */
void reshape (int w, int h)
{
	int winRes = h;
	ratio = 1.0f * w / h;
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, ratio, 0.01, 1000);
	glMatrixMode(GL_MODELVIEW);
	setCamera();
} //end reshape


/* Main Loop */
int main(int argc, char** argv)
{

	// GLUT Initialization
	glutInit(&argc, argv);

	// Create window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0,0);
	mainWindowId = glutCreateWindow("Constraint Particle System - Vishak Nag Ashoka");

	// Initialize Application
	initialize();

	// Init Texture
	textureInit();

	// Redraws window if window is resized
	glutReshapeFunc(reshape);

	// Create GLUI window
	createGLUI();

	// Initialize the Physics module
	pauseSimulation = 1;
	ParticleSystemInit();

	// Display Graphics
	glutDisplayFunc(display);
	GLUI_Master.set_glutIdleFunc(idle);

	// Read User Mouse Input
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);

	// Read User Keyboard Input
	glutKeyboardFunc(keyboardKeys);	
	glutSpecialFunc(specialKeys);

	glutMainLoop();
	return 0; 
}