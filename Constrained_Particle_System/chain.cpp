#include "chain.h"
#include "render.h"
#include "physics.h"

GLUquadricObj *quadratic;

void DrawRing()
{
	GLUquadricObj *quadratic;
	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )

	GLfloat materialColor2[4];
	if(materialColor == GOLD)
	{
		materialColor2[0] = 1.0;
		materialColor2[1] = 1.0;
		materialColor2[2] = 0.0;
		materialColor2[3] = 1.0;
	}
	if(materialColor == MAROON)
	{
		materialColor2[0] = 0.556863;
		materialColor2[1] = 0.137255;
		materialColor2[2] = 0.419608;
		materialColor2[3] = 1.0;
	}
	if(materialColor == ORANGE)
	{
		materialColor2[0] = 1.0;
		materialColor2[1] = 0.5;
		materialColor2[2] = 0.0;
		materialColor2[3] = 1.0;
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialColor2);

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	gluDisk(quadratic, 0.5f, 0.54f, 60, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.75, 0);
	gluDisk(quadratic, 0.75f, 0.79f, 60, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -1.0, 0);
	gluDisk(quadratic, 1.0f, 1.04f, 60, 32);
	glPopMatrix();
}
	

void DrawParticles()
{
	GLfloat materialColor2[4];

	//The color of the SPHERE Object in the bounding box
	if(THEME1)
	{
		materialColor2[0] = 1.0;
		materialColor2[1] = 0.43;
		materialColor2[2] = 0.78;
		materialColor2[3] = 1.0;		// Green yellow
	}
	else if(THEME2)
	{
		materialColor2[0] = 1.0;
		materialColor2[1] = 0.43;
		materialColor2[2] = 0.430;
		materialColor2[3] = 1.0;	
	}
	else if(THEME3)
	{
		materialColor2[0] = 1.0;
		materialColor2[1] = 0.43;
		materialColor2[2] = 0.430;
		materialColor2[3] = 1.0;	
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialColor2);
	
	for(int i = 0; i < pSystemObj->pCount; i++)
	{
		glPushMatrix();
		glTranslatef(pSystemObj->stateVector[i*2], pSystemObj->stateVector[(i*2)+1], 0.0);
		glutSolidSphere(0.04, 150, 100);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glutSolidSphere(0.04, 150, 100);
	glPopMatrix();

}

double RadToDeg(double angleR)
{
	return (180 * angleR) / PI;
}


void DrawCylinders()
{
	double xDiff = 0.0, yDiff = 0.0, zDiff = 0, angleD = 0.0, angleR = 0.0, value = 0.0;
	double radX = 0.0, radY = 0.0;

	quadratic=gluNewQuadric();					// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )	

	GLfloat materialColor3[4];

	//The color of the SPHERE Object in the bounding box
	if(THEME1)
	{
		materialColor3[0] = 1.0;
		materialColor3[1] = 0.0;
		materialColor3[2] = 0.0;
		materialColor3[3] = 1.0;		// Red
	}
	else if(THEME2)
	{
		materialColor3[0] = 1.0;
		materialColor3[1] = 0.43;
		materialColor3[2] = 0.430;
		materialColor3[3] = 1.0;	
	}
	else if(THEME3)
	{
		materialColor3[0] = 1.0;
		materialColor3[1] = 0.43;
		materialColor3[2] = 0.430;
		materialColor3[3] = 1.0;	
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialColor3);

	xDiff = pSystemObj->stateVector[0] - 0.0;
	yDiff = pSystemObj->stateVector[1] - 0.0;
	zDiff = 0.0;

	if(zDiff == 0)
	   zDiff = .00000001;

	value = sqrt( xDiff*xDiff + yDiff*yDiff + zDiff*zDiff );
	angleR = 57.2957795*acos( zDiff/value );

	if ( zDiff < 0.0 )
		angleR = -angleR;

	radX = -yDiff*zDiff;
	radY = xDiff*zDiff;

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(angleR, radX, radY, 0.0);
	gluCylinder(quadratic, 0.01f, 0.01f, value, 20, 1);	
	glPopMatrix();

	for(int i = 0; i < pSystemObj->degOfFreedom-1; i+=2)
	{
		xDiff = pSystemObj->stateVector[i+2] - pSystemObj->stateVector[i];
		yDiff = pSystemObj->stateVector[i+1+2] - pSystemObj->stateVector[i+1];
		zDiff = 0.0;

		if(zDiff == 0)
		   zDiff = .00000001;

		value = sqrt( xDiff*xDiff + yDiff*yDiff + zDiff*zDiff );
		angleR = 57.2957795*acos( zDiff/value );

		if ( zDiff < 0.0 )
			angleR = -angleR;

		radX = -yDiff*zDiff;
		radY = xDiff*zDiff;

		glPushMatrix();
		glTranslatef(pSystemObj->stateVector[i], pSystemObj->stateVector[i+1], 0.0);
		glRotatef(angleR, radX, radY, 0.0);
		gluCylinder(quadratic, 0.01f, 0.01f, value, 20, 1);	
		glPopMatrix();
	}
}


void DrawPSystem()
{
	DrawRing();
	DrawParticles();
	DrawCylinders();
}