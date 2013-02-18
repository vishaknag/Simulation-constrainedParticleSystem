/* Source: world
 * Description: Contains functions that renders background and/or environment objects.
 */

#include "render.h"
#include "world.h"
#include "chain.h"

double tallY = -1.2;
double shortY = -1.5;
int Animate = 0;
double wallShort = 0.75;
double wallTall = 0.77;

// Dark Purple = glColor4f(0.53, 0.12, 0.47, 1.0);
// Neon Pink = glColor4f(1.0, 0.43, 0.78, 1.0);

/* Function: worldInit
 * Description: Initializes all background and/or environment object variables and settings.
 * Input: None
 * Output: None
 */
void worldInit()
{
	//Initialize variables
} //end worldInit

/* Function: renderAxis
 * Description: Renders the xyz Axis in world space.
 * Input: None
 * Output: None
 */
void renderAxis()
{
	glLineWidth(2);
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glBegin(GL_LINES);
			// Render X Axis
			glColor4f(1.0, 0.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(1.0, 0.0, 0.0);

			// Render Y Axis
			glColor4f(0.0, 1.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, 1.0, 0.0);

			// Render Z Axis
			glColor4f(0.0, 0.0, 1.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, 1.0);
		glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
} //end renderAxis


/* Function: setBoundingBox
 * Description: Setup the normals of each face in the bounding box and its distance to world origin.
 * Input: box - Data structure of the bounding box
 * Output: Nothing
 */
void setBoundingBox(struct boundBox * box)
{
	// Set Left Face Information
	box->norm[0].x = 1.0;
	box->norm[0].y = 0.0;
	box->norm[0].z = 0.0;
	box->d[0] = 2.0;

	// Set Right Face Information
	box->norm[1].x = -1.0;
	box->norm[1].y = 0.0;
	box->norm[1].z = 0.0;
	box->d[1] = 2.0;

	// Set Front Face Information
	box->norm[2].x = 0.0;
	box->norm[2].y = 0.0;//1.0;
	box->norm[2].z = -1;//0.0
	box->d[2] = 2.0;

	// Set Back Face Information
	box->norm[3].x = 0.0;
	box->norm[3].y = 0.0;//-1.0;
	box->norm[3].z = 1;//0.0
	box->d[3] = 2.0;

	// Set Bottom Face Information
	box->norm[4].x = 0.0;
	box->norm[4].y = 1;//0.0
	box->norm[4].z = 0.0;//1.0;
	box->d[4] = 2.0;

	// Set Top Face Information
	box->norm[5].x = 0.0;
	box->norm[5].y = -1;//0.0;
	box->norm[5].z = 0.0;//-1.0;
	box->d[5] = 2.0;
} //end boundingBox


void DrawFrontFace()
{
	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-2.0, tallY, 2.0);
		glVertex3f(-2.0, -2.0, 2.0);
		glVertex3f(-1.5, -2.0, 2.0);
		glVertex3f(-1.5, tallY, 2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-1.5, shortY, 2.0);
		glVertex3f(-1.5, -2.0, 2.0);
		glVertex3f(-1.0, -2.0, 2.0);
		glVertex3f(-1.0, shortY, 2.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-1.0, tallY, 2.0);
		glVertex3f(-1.0, -2.0, 2.0);
		glVertex3f(-0.5, -2.0, 2.0);
		glVertex3f(-0.5, tallY, 2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-0.5, shortY, 2.0);
		glVertex3f(-0.5, -2.0, 2.0);
		glVertex3f(-0.0, -2.0, 2.0);
		glVertex3f(-0.0, shortY, 2.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-0.0, tallY, 2.0);
		glVertex3f(-0.0, -2.0, 2.0);
		glVertex3f(0.5, -2.0, 2.0);
		glVertex3f(0.5, tallY, 2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(0.5, shortY, 2.0);
		glVertex3f(0.5, -2.0, 2.0);
		glVertex3f(1.0, -2.0, 2.0);
		glVertex3f(1.0, shortY, 2.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(1.0, tallY, 2.0);
		glVertex3f(1.0, -2.0, 2.0);
		glVertex3f(1.5, -2.0, 2.0);
		glVertex3f(1.5, tallY, 2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(1.5, shortY, 2.0);
		glVertex3f(1.5, -2.0, 2.0);
		glVertex3f(2.0, -2.0, 2.0);
		glVertex3f(2.0, shortY, 2.0);
	glEnd();
}


void DrawRightFace()
{
	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(2.0, tallY, -2.0);
		glVertex3f(2.0, -2.0, -2.0);
		glVertex3f(2.0, -2.0, -1.5);
		glVertex3f(2.0, tallY, -1.5);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(2.0, shortY, -1.5);
		glVertex3f(2.0, -2.0, -1.5);
		glVertex3f(2.0, -2.0, -1.0);
		glVertex3f(2.0, shortY, -1.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(2.0, tallY, -1.0);
		glVertex3f(2.0, -2.0, -1.0);
		glVertex3f(2.0, -2.0, -0.5);
		glVertex3f(2.0, tallY, -0.5);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(2.0, shortY, -0.5);
		glVertex3f(2.0, -2.0, -0.5);
		glVertex3f(2.0, -2.0, 0.0);
		glVertex3f(2.0, shortY, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(2.0, tallY, 0.0);
		glVertex3f(2.0, -2.0, 0.0);
		glVertex3f(2.0, -2.0, 0.5);
		glVertex3f(2.0, tallY, 0.5);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(2.0, shortY, 0.5);
		glVertex3f(2.0, -2.0, 0.5);
		glVertex3f(2.0, -2.0, 1.0);
		glVertex3f(2.0, shortY, 1.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(2.0, tallY, 1.0);
		glVertex3f(2.0, -2.0, 1.0);
		glVertex3f(2.0, -2.0, 1.5);
		glVertex3f(2.0, tallY, 1.5);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(2.0, shortY, 1.5);
		glVertex3f(2.0, -2.0, 1.5);
		glVertex3f(2.0, -2.0, 2.0);
		glVertex3f(2.0, shortY, 2.0);
	glEnd();
}


void DrawLeftFace()
{
	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-2.0, tallY, -2.0);
		glVertex3f(-2.0, -2.0, -2.0);
		glVertex3f(-2.0, -2.0, -1.5);
		glVertex3f(-2.0, tallY, -1.5);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-2.0, shortY, -1.5);
		glVertex3f(-2.0, -2.0, -1.5);
		glVertex3f(-2.0, -2.0, -1.0);
		glVertex3f(-2.0, shortY, -1.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-2.0, tallY, -1.0);
		glVertex3f(-2.0, -2.0, -1.0);
		glVertex3f(-2.0, -2.0, -0.5);
		glVertex3f(-2.0, tallY, -0.5);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-2.0, shortY, -0.5);
		glVertex3f(-2.0, -2.0, -0.5);
		glVertex3f(-2.0, -2.0, 0.0);
		glVertex3f(-2.0, shortY, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-2.0, tallY, 0.0);
		glVertex3f(-2.0, -2.0, 0.0);
		glVertex3f(-2.0, -2.0, 0.5);
		glVertex3f(-2.0, tallY, 0.5);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-2.0, shortY, 0.5);
		glVertex3f(-2.0, -2.0, 0.5);
		glVertex3f(-2.0, -2.0, 1.0);
		glVertex3f(-2.0, shortY, 1.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-2.0, tallY, 1.0);
		glVertex3f(-2.0, -2.0, 1.0);
		glVertex3f(-2.0, -2.0, 1.5);
		glVertex3f(-2.0, tallY, 1.5);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-2.0, shortY, 1.5);
		glVertex3f(-2.0, -2.0, 1.5);
		glVertex3f(-2.0, -2.0, 2.0);
		glVertex3f(-2.0, shortY, 2.0);
	glEnd();
}


void DrawBackFace()
{
	glBegin(GL_POLYGON);
		glColor4f(0.8, 0.8, 0.8, 1.0);
		glVertex3f(-2.0, tallY, -2.0);
		glVertex3f(-2.0, -2.0, -2.0);
		glVertex3f(-1.5, -2.0, -2.0);
		glVertex3f(-1.5, tallY, -2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-1.5, shortY, -2.0);
		glVertex3f(-1.5, -2.0, -2.0);
		glVertex3f(-1.0, -2.0, -2.0);
		glVertex3f(-1.0, shortY, -2.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-1.0, tallY, -2.0);
		glVertex3f(-1.0, -2.0, -2.0);
		glVertex3f(-0.5, -2.0, -2.0);
		glVertex3f(-0.5, tallY, -2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-0.5, shortY, -2.0);
		glVertex3f(-0.5, -2.0, -2.0);
		glVertex3f(-0.0, -2.0, -2.0);
		glVertex3f(-0.0, shortY, -2.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-0.0, tallY, -2.0);
		glVertex3f(-0.0, -2.0, -2.0);
		glVertex3f(0.5, -2.0, -2.0);
		glVertex3f(0.5, tallY, -2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(0.5, shortY, -2.0);
		glVertex3f(0.5, -2.0, -2.0);
		glVertex3f(1.0, -2.0, -2.0);
		glVertex3f(1.0, shortY, -2.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(1.0, tallY, -2.0);
		glVertex3f(1.0, -2.0, -2.0);
		glVertex3f(1.5, -2.0, -2.0);
		glVertex3f(1.5, tallY, -2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(1.5, shortY, -2.0);
		glVertex3f(1.5, -2.0, -2.0);
		glVertex3f(2.0, -2.0, -2.0);
		glVertex3f(2.0, shortY, -2.0);
	glEnd();
}


void DrawPillars()
{
	// FRONT LEFT PILLAR
	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-2.0, wallShort, 2.0);
		glVertex3f(-2.0, -2.0, 2.0);
		glVertex3f(-1.7, -2.0, 2.0);
		glVertex3f(-1.7, wallTall, 2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-1.7, wallTall, 2.0);
		glVertex3f(-1.7, -2.0, 2.0);
		glVertex3f(-1.7, -2.0, 1.7);
		glVertex3f(-1.7, wallTall, 1.7);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-1.7, wallTall, 1.7);
		glVertex3f(-1.7, -2.0, 1.7);
		glVertex3f(-2.0, -2.0, 1.7);
		glVertex3f(-2.0, wallShort, 1.7);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-2.0, wallShort, 1.7);
		glVertex3f(-2.0, -2.0, 1.7);
		glVertex3f(-2.0, -2.0, 2.0);
		glVertex3f(-2.0, wallShort, 2.0);
	glEnd();

	// FRONT RIGHT PILLAR
	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(2.0, wallShort, 2.0);
		glVertex3f(2.0, -2.0, 2.0);
		glVertex3f(1.7, -2.0, 2.0);
		glVertex3f(1.7, wallTall, 2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(1.7, wallTall, 2.0);
		glVertex3f(1.7, -2.0, 2.0);
		glVertex3f(1.7, -2.0, 1.7);
		glVertex3f(1.7, wallTall, 1.7);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(1.7, wallTall, 1.7);
		glVertex3f(1.7, -2.0, 1.7);
		glVertex3f(2.0, -2.0, 1.7);
		glVertex3f(2.0, wallShort, 1.7);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(2.0, wallShort, 1.7);
		glVertex3f(2.0, -2.0, 1.7);
		glVertex3f(2.0, -2.0, 2.0);
		glVertex3f(2.0, wallShort, 2.0);
	glEnd();

	// BACK LEFT PILLAR
	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-2.0, wallShort, -2.0);
		glVertex3f(-2.0, -2.0, -2.0);
		glVertex3f(-1.7, -2.0, -2.0);
		glVertex3f(-1.7, wallTall, -2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-1.7, wallTall, -2.0);
		glVertex3f(-1.7, -2.0, -2.0);
		glVertex3f(-1.7, -2.0, -1.7);
		glVertex3f(-1.7, wallTall, -1.7);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(-1.7, wallTall, -1.7);
		glVertex3f(-1.7, -2.0, -1.7);
		glVertex3f(-2.0, -2.0, -1.7);
		glVertex3f(-2.0, wallShort, -1.7);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(-2.0, wallShort, -1.7);
		glVertex3f(-2.0, -2.0, -1.7);
		glVertex3f(-2.0, -2.0, -2.0);
		glVertex3f(-2.0, wallShort, -2.0);
	glEnd();

	// BACK RIGHT PILLAR
	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, 1.0);
		glVertex3f(2.0, wallShort, -2.0);
		glVertex3f(2.0, -2.0, -2.0);
		glVertex3f(1.7, -2.0, -2.0);
		glVertex3f(1.7, wallTall, -2.0);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(1.7, wallTall, -2.0);
		glVertex3f(1.7, -2.0, -2.0);
		glVertex3f(1.7, -2.0, -1.7);
		glVertex3f(1.7, wallTall, -1.7);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.6, 0.8, 0.196078, -1.0);
		glVertex3f(1.7, wallTall, -1.7);
		glVertex3f(1.7, -2.0, -1.7);
		glVertex3f(2.0, -2.0, -1.7);
		glVertex3f(2.0, wallShort, -1.7);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.22, 0.69, 0.87, 1.0);
		glVertex3f(2.0, wallShort, -1.7);
		glVertex3f(2.0, -2.0, -1.7);
		glVertex3f(2.0, -2.0, -2.0);
		glVertex3f(2.0, wallShort, -2.0);
	glEnd();

}


/* Function: renderBoundingBox
 * Description: Renders the bounding box encompassing the object.
 * Input: None
 * Output: None
 */
void renderBoundingBox()
{
	
	GLUquadricObj *quadratic;
	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )

	// Render wall in wireframe
	glDisable(GL_LIGHTING);

	glCullFace(GL_FRONT);
	DrawFrontFace();
	DrawRightFace();
	DrawLeftFace();
	DrawBackFace();
	DrawPillars();

	GLfloat materialColor1[4];
	//The color of the SPHERE Object in the bounding box
	if(THEME1)
	{
		materialColor1[0] = 0.42;
		materialColor1[1] = 0.26;
		materialColor1[2] = 0.15;
		materialColor1[3] = 1.0;		
	}
	else if(THEME2)
	{
		materialColor1[0] = 0.576471;
		materialColor1[1] = 0.858824;
		materialColor1[2] = 0.430;
		materialColor1[3] = 1.0;	
	}
	else if(THEME3)
	{
		materialColor1[0] = 0.576471;
		materialColor1[1] = 0.858824;
		materialColor1[2] = 0.430;
		materialColor1[3] = 1.0;	
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialColor1);

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	gluDisk(quadratic, 2.0f, 2.0f, 100, 32);
	glPopMatrix();

	glEnable(GL_LIGHTING);
} //end renderBoundingBox