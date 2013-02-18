/* Source: physics
 * Description: Contains functions that compute physics in real time.
 */
//ORIGINAL
#include <gsl/gsl_linalg.h>
#include "render.h"
#include "physics.h"
#include "matrix.h"
#include "world.h"
#include <fstream>
#include <iostream>
using namespace std;


#define INITPOSX 0
#define INITPOSY 0
#define INITPOSZ 0

#define CDER1 1
#define CDER2 2
#define CDER3 3
#define CDER4 4
#define CDERRINGX 5
#define CDERRINGY 6

#define CDOTDER1 1
#define CDOTDER2 2
#define CDOTDER3 3
#define CDOTDER4 4
#define CDOTDERRINGX 5
#define CDOTDERRINGY 6

#define PINX 0.0
#define PINY 0.0
#define ALPHA 2*sqrt(BETA)

#define CREATEFILE 0

float BETA = 0.9;
float UNDERWATER_ALPHA = 0.1;
int PCOUNT = 11;
double pRINGX = 0.0, pRINGY = -0.5;
int  UNDERWATER = 1;
float GRAVITY = 1.0;
double PULLX = 0.0;
int stop = 0;
float TIMESTEP = 0.003;
int RUNTEST = 0;

particleSystem *pSystemObj;
ofstream values_file;

// Constructor
particleSystem::particleSystem(int numParticles)		// By default numParticles is 11
{	
	n = NSTEP;	
	pCount = numParticles-1;							// Do not consider the first particle
	degOfFreedom = pCount * 2;					// 2D - each particle has two degrees of freedom

	numConstraints = (pCount - 1) + 1 + 1;		// N-1 RIGID Constraints
														// 1   RING Constraint
														// 1   PIN Constraint(RIGID FORM)

	pMass = (1.0 / pCount);						// Mass of each particle is 1 / N
	linkLength = ((-2 * pRINGY) / (numParticles - 1.0));			// Length of each link is 1 / N-1
	h = TIMESTEP;										// timestep for the integrator
	
	pPosition = (struct point *)malloc(pCount*sizeof(struct point));				// Current Positions of all the particles

	matM = (double *)malloc(degOfFreedom*degOfFreedom*sizeof(double));
	matGradC = (double *)malloc(numConstraints*degOfFreedom*sizeof(double));
	matGradCDot = (double *)malloc(numConstraints*degOfFreedom*sizeof(double));
	matGradCTrans = (double *)malloc(degOfFreedom*numConstraints*sizeof(double));
	matZero = (double *)malloc(numConstraints*numConstraints*sizeof(double));
	
	rowcolA = degOfFreedom + numConstraints;
	A = (double**)malloc(rowcolA * sizeof(double *));				// Allocating for row pointers
	for (int i = 0; i < rowcolA; i++)
		A[i] = (double*)malloc(rowcolA * sizeof(double));			// Allocating every row 

	B = (double*)malloc(rowcolA * sizeof(double));

	stateVector = (double *)malloc(degOfFreedom*sizeof(double));
	stateVelocity = (double *)malloc(degOfFreedom*sizeof(double));
	matCDotQDot = (double *)malloc(numConstraints*sizeof(double));
	matCQDot = (double *)malloc(numConstraints*sizeof(double));
	C = (double *)malloc(numConstraints*sizeof(double));
	matFExt = (double *)malloc(degOfFreedom*sizeof(double));
	matAcc = (double *)malloc(degOfFreedom*sizeof(double));

	memset(matM, 0, sizeof(double)*degOfFreedom*degOfFreedom);
	memset(matGradC, 0, sizeof(double) * numConstraints*degOfFreedom);
	memset(matGradCDot, 0, sizeof(double) * numConstraints*degOfFreedom);
	memset(matGradCDot, 0, sizeof(double) * numConstraints*degOfFreedom);
	memset(matGradCTrans, 0, sizeof(double) * degOfFreedom*numConstraints);
	memset(matZero, 0, sizeof(double) * numConstraints*numConstraints);
	memset(stateVector, 0, sizeof(double) * degOfFreedom);
	memset(stateVelocity, 0, sizeof(double) * degOfFreedom);
	memset(matCDotQDot, 0, sizeof(double) * numConstraints);
	memset(matCQDot, 0, sizeof(double) * numConstraints);
	memset(B, 0, sizeof(double) * rowcolA);
	memset(C, 0, sizeof(double) * numConstraints);
	memset(pPosition, 0, (pCount)*sizeof(struct point));
	memset(matFExt, 0, sizeof(double) * degOfFreedom);
	memset(matAcc, 0, sizeof(double) * degOfFreedom);
}


/* Function: ParticleSystemInit
 * Description: Creates and initializes the ParticleSystem object
 * Input: None
 * Output: None
 */
void ParticleSystemInit()
{
	
	// New Physics object
	pSystemObj = new particleSystem(PCOUNT);
	
	// Create a file for storing the output C vector
	if(CREATEFILE)
	{
		CreateFile();
	}

	if(RUNTEST == 0)
	{
		// Set positions for the particles from top to center of the ring
		for(int i = 0; i < (pSystemObj->pCount)/2 ; i++)
		{
			pSystemObj->pPosition[i].x = INITPOSX;
			pSystemObj->pPosition[i].y = INITPOSY - (pSystemObj->linkLength * (i+1));
			pSystemObj->pPosition[i].z = INITPOSZ;
		}

		// Set positions for the particles from center of the ring to the right of the ring
		int j = 1;
		for(int i = (pSystemObj->pCount)/2; i < (pSystemObj->pCount); i++)
		{
			pSystemObj->pPosition[i].x = INITPOSX + (pSystemObj->linkLength * j);
			pSystemObj->pPosition[i].y = INITPOSY + pRINGY;
			pSystemObj->pPosition[i].z = INITPOSZ;
			j++;
		}
	}
	else if(RUNTEST == 1)
	{
		// Set positions for the particles from top to center of the ring
		for(int i = 0; i < (pSystemObj->pCount) ; i++)
		{
			pSystemObj->pPosition[i].x = INITPOSX + (pSystemObj->linkLength * (i+1));
			pSystemObj->pPosition[i].y = INITPOSY;
			pSystemObj->pPosition[i].z = INITPOSZ;
		}
	}
	
	for(int i = 0; i < (pSystemObj->pCount); i++)
	{
		// All the x, y positions are filled in the stateVector used for finding gradient C
		pSystemObj->stateVector[i*2] = pSystemObj->pPosition[i].x;
		pSystemObj->stateVector[i*2+1] = pSystemObj->pPosition[i].y;
		
		// Fill the state velocity vector
		pSystemObj->stateVelocity[i*2] = 0.0;			// velocity x 
		pSystemObj->stateVelocity[i*2+1] = 0.0;			// velocity y
	}
}


/* Function: FindIndexInMatrix
 * Description: Returns the index of the matrix array
 * Input: None
 * Output: None
 */
int FindIndexInMatrix(int row, int col, int sizeOfRow)
{
	int resultIndex = 0;
	resultIndex = (row * sizeOfRow) + col;

	return resultIndex;
}



/* Function: FillMatM
 * Description: Fills the matrix M
 * Input: None
 * Output: None
 */
void FillMatM()
{
	int matArrIndex = 0;

	for(int i = 0; i < (pSystemObj->degOfFreedom*pSystemObj->degOfFreedom); i++)
	{
		pSystemObj->matM[i] = 0.0;
	}

	for(int index = 0; index < pSystemObj->degOfFreedom; index++)
	{
		matArrIndex = FindIndexInMatrix(index, index, pSystemObj->degOfFreedom);
		pSystemObj->matM[matArrIndex] = pSystemObj->pMass;	// Currently all the particles are of same mass
	}
}


// C Gradient Derivatives
double CDerivatives(double x1Ory1, double x2Ory2, int derivative)
{
	double result = 0.0;

	switch(derivative)
	{
		case CDER1:		// -2(x2-x1)
			 result = (-2.0 * (x2Ory2 - x1Ory1));
				break;

		case CDER2:		// -2(y2-y1)
			 result = (-2.0 * (x2Ory2 - x1Ory1));
				break;

		case CDER3:		// 2(x2-x1)
			 result = (2.0 * (x2Ory2 - x1Ory1));
				break;

		case CDER4:		// 2(y2-y1)
			 result = (2.0 * (x2Ory2 - x1Ory1));
				break;

		case CDERRINGX:	// 2(x1 - pRINGX)
			 result = (2.0 * (x1Ory1 - pRINGX));
				break;

		case CDERRINGY:	// 2(x1 - pRINGY)
			 result = (2.0 * (x1Ory1 - pRINGY));
				break;
	}	
	if(result == -0.0)
		return 0.0;
	else 
		return result;
}


// C Dot Derivatives
double CDotDerivatives(double x1DotOry1Dot, double x2DotOry2Dot, int derivative)
{

	double result = 0.0;

	switch(derivative)
	{
		case CDOTDER1:		// -2(x2Dot - xDot1)
			result = (-2.0 * (x2DotOry2Dot - x1DotOry1Dot));
				break;

		case CDOTDER2:		// -2(y2Dot - yDot1)
			result = (-2.0 * (x2DotOry2Dot - x1DotOry1Dot));
				break;

		case CDOTDER3:		// 2(x2Dot - xDot1)
			result = (2.0 * (x2DotOry2Dot - x1DotOry1Dot));
				break;

		case CDOTDER4:		// -2(y2Dot - yDot1)
			result = (2.0 * (x2DotOry2Dot - x1DotOry1Dot));
				break;

		case CDOTDERRINGX:	// 2(x1Dot)
			result = (2.0 * x1DotOry1Dot);
				break;

		case CDOTDERRINGY:	// 2(y1Dot)
			result = (2.0 * x1DotOry1Dot);
				break;
	}	
	if(result == -0.0)
		return 0.0;
	else 
		return result;
}


/* Function: FillMatGradC
 * Description: Fills the matrix matGradC
 * Input: None
 * Output: None
 */
void FillMatGradC()
{
	int row = 0, col = 0;
	int index = 0;
	double x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;
	int startFill = 0, endFill = 0;
	
	for(row = 0; row < (pSystemObj->numConstraints-2); row++)
	{
		startFill = -1;
		endFill = -1;

		for(col = 0; col < pSystemObj->degOfFreedom; col++)
		{
			if( col == (row * 2))
			{
				// Start of the blocks in the current row
				startFill = col;
				endFill = col+3;

				index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);
					
				x1 = pSystemObj->stateVector[col];
				y1 = pSystemObj->stateVector[col+1];
				x2 = pSystemObj->stateVector[col+2];
				y2 = pSystemObj->stateVector[col+3];

				pSystemObj->matGradC[index] = CDerivatives(x1, x2, CDER1);
				pSystemObj->matGradC[index+1] = CDerivatives(y1, y2, CDER2);
				pSystemObj->matGradC[index+2] = CDerivatives(x1, x2, CDER3);
				pSystemObj->matGradC[index+3] = CDerivatives(y1, y2, CDER4);
			}
			else	// Fill with zeroes in all other positions of the matrix current row
			{
				// if any of the other 3 valid blocks then dont fill with zeroes
				if((col >= startFill) && (col <= endFill))
				{
					continue;		
				}
				else	// You are all set to fill with zeroes :)
				{
					index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);
					pSystemObj->matGradC[index] = +0.0;
				}
			}
			
			// Make -Zero to + zero
			index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);
			if(pSystemObj->matGradC[index] == -0.0)
			{
				pSystemObj->matGradC[index] = +0.0;
			}

		} // for col
	} // for row

	// PIN CONSTRAINT FOR THE FIRST PARTICLE (RIGID FORM)
	for(col = 0; col < pSystemObj->degOfFreedom; col++)
	{
		index = FindIndexInMatrix(pSystemObj->numConstraints-2, col, pSystemObj->degOfFreedom);
		pSystemObj->matGradC[index] = +0.0;
	}

	index = FindIndexInMatrix(pSystemObj->numConstraints-2, 0, pSystemObj->degOfFreedom);
	x2 = pSystemObj->stateVector[0];
	y2 = pSystemObj->stateVector[1];
	
	pSystemObj->matGradC[index] = CDerivatives(PINX, x2, CDER3);
	pSystemObj->matGradC[index+1] = CDerivatives(PINY, y2, CDER4);

	// RING CONSTRAINT FOR THE LAST PARTICLE
	for(col = 0; col < pSystemObj->degOfFreedom; col++)
	{
		index = FindIndexInMatrix(pSystemObj->numConstraints-1, col, pSystemObj->degOfFreedom);
		pSystemObj->matGradC[index] = +0.0;
	}

	if(RING)
	{
		index = FindIndexInMatrix(pSystemObj->numConstraints-1, pSystemObj->degOfFreedom-2, pSystemObj->degOfFreedom);
		x1 = pSystemObj->stateVector[pSystemObj->degOfFreedom-2];		// Nth x
		y1 = pSystemObj->stateVector[pSystemObj->degOfFreedom-1];		// Nth y

		pSystemObj->matGradC[index] = CDerivatives(x1, 0, CDERRINGX);
		pSystemObj->matGradC[index+1] = CDerivatives(y1, 0, CDERRINGY);
	}

}


void CreateFile()
{
	ofstream values_file("Constraint.csv");

	OpenFile();
	for(int row = 0; row < (pSystemObj->numConstraints); row++)
	{
		if(row != pSystemObj->numConstraints-1)
		{
			values_file<<"Constraint "<<row+1<<",";
		}
		else
		{
			values_file<<"Constraint "<<row+1<<endl;
		}
	}
	CloseFile();
}

void OpenFile()
{
	values_file.open("Constraint.csv", ifstream::app);
}

void CloseFile()
{
	values_file.close();
}

void PrintMat()
{
	int index = 0;
/*
	// MAT M
	printf("\n\nMAT M\n\n");
	for(int row = 0; row < (pSystemObj->degOfFreedom); row++)
	{
		for(int col = 0; col < (pSystemObj->degOfFreedom); col++)
		{
			index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);

			if(pSystemObj->matM[index] == 0.0)
				printf("%d ", 0);
			else
				printf("%.2lf ", pSystemObj->matM[index]);
		}
		printf("\n");	
	}

	// MAT GRADIENT C
	printf("\n\nMAT GRADIENT C\n\n");
	for(int row = 0; row < (pSystemObj->numConstraints); row++)
	{
		for(int col = 0; col < (pSystemObj->degOfFreedom); col++)
		{
			index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);

			if(pSystemObj->matGradC[index] == 0.0)
				printf("%d ", 0);
			else
			printf("%.2lf ", pSystemObj->matGradC[index]);
		}
		printf("\n");	
	}

	// MAT GRADIENT C TRANS
	printf("\n\nMAT GRADIENT C TRANS\n\n");
	for(int row = 0; row < (pSystemObj->degOfFreedom); row++)
	{
		for(int col = 0; col < (pSystemObj->numConstraints); col++)
		{
			index = FindIndexInMatrix(row, col, pSystemObj->numConstraints);

			if(pSystemObj->matGradCTrans[index] == 0.0)
				printf("%d ", 0);
			else
			printf("%.2lf ", pSystemObj->matGradCTrans[index]);
		}
		printf("\n");	
	}

	// MAT ZERO
	printf("\n\nMAT ZERO\n\n");
	for(int row = 0; row < (pSystemObj->numConstraints); row++)
	{
		for(int col = 0; col < (pSystemObj->numConstraints); col++)
		{
			index = FindIndexInMatrix(row, col, pSystemObj->numConstraints);

			if(pSystemObj->matZero[index] == 0.0)
				printf("%d ", 0);
			else
			printf("%.2lf ", pSystemObj->matZero[index]);
		}
		printf("\n");	
	}

	// VECTOR FEXT
	printf("\n\nVECTOR FEXT\n\n");
	for(int row = 0; row < (pSystemObj->degOfFreedom); row++)
	{
		if(pSystemObj->matFExt[row] == 0.0)
				printf("%d ", 0);
			else
		printf("%.2lf ", pSystemObj->matFExt[row]);
		printf("\n");	
	}

	// VECTOR POSITIONS
	printf("\n\nVECTOR POSITIONS\n\n");
	for(int row = 0; row < (pSystemObj->degOfFreedom); row++)
	{
		if(pSystemObj->stateVector[row] == 0.0)
				printf("%d ", 0);
			else
		printf("%.2lf ", pSystemObj->stateVector[row]);
		printf("\n");	
	}

	// VECTOR Q DOT
	printf("\n\nVECTOR Q DOT\n\n");
	for(int row = 0; row < (pSystemObj->degOfFreedom); row++)
	{
		if(pSystemObj->stateVelocity[row] == 0.0)
				printf("%d ", 0);
			else
		printf("%.2lf ", pSystemObj->stateVelocity[row]);
		printf("\n");	
	}

	// MAT C DOT Q DOT
	printf("\n\nMAT C DOT Q DOT\n\n");
	for(int row = 0; row < (pSystemObj->numConstraints); row++)
	{
		if(pSystemObj->matCDotQDot[row] == 0.0)
				printf("%d ", 0);
			else
		printf("%.2lf ", pSystemObj->matCDotQDot[row]);
		printf("\n");	
	}

	// VECTOR GRADIENT C Q DOT
	printf("\n\nVECTOR GRADIENT C Q DOT\n\n");
	for(int row = 0; row < (pSystemObj->numConstraints); row++)
	{
		if(pSystemObj->matCQDot[row] == 0.0)
				printf("%d ", 0);
			else
		printf("%.2lf ", pSystemObj->matCQDot[row]);
		printf("\n");	
	}

	// MAT GRADIENT C DOT
	printf("\n\nMAT GRADIENT C DOT\n\n");
	for(int row = 0; row < (pSystemObj->numConstraints); row++)
	{
		for(int col = 0; col < (pSystemObj->degOfFreedom); col++)
		{
			index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);

			if(pSystemObj->matGradCDot[index] == 0.0)
				printf("%d ", 0);
			else
			printf("%.2lf ", pSystemObj->matGradCDot[index]);
		}
		printf("\n");	
	}

	// MAT A
	printf("\n\nMAT A\n\n");
	for(int row = 0; row < (pSystemObj->numConstraints+pSystemObj->degOfFreedom); row++)
	{
		for(int col = 0; col < (pSystemObj->degOfFreedom+pSystemObj->numConstraints); col++)
		{
			if(pSystemObj->A[row][col] == 0.0)
				printf("%d ", 0);
			else
			printf("%.2lf ", pSystemObj->A[row][col]);
		}
		printf("\n");	
	}
	*/

	if(CREATEFILE)
	{
		OpenFile();

		for(int row = 0; row < (pSystemObj->numConstraints); row++)
		{
			if(row != pSystemObj->numConstraints-1)
			{
				values_file<<pSystemObj->C[row]<<",";
			}
			else
			{
				values_file<<pSystemObj->C[row]<<endl;
			}
		}
		CloseFile();
	}
}


/* Function: FillMatGradCTrans
 * Description: Fills the matrix matGradCTrans
 * Input: None
 * Output: None
 */
void FillMatGradCTrans()
{
	int row = 0, col = 0;
	int CIndex = 0, CTransIndex = 0;

	for(row = 0; row < pSystemObj->numConstraints; row++)
	{
		for(col = 0; col < pSystemObj->degOfFreedom; col++)
		{
			CIndex = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);
			CTransIndex = FindIndexInMatrix(col, row, pSystemObj->numConstraints);

			pSystemObj->matGradCTrans[CTransIndex] = pSystemObj->matGradC[CIndex];

		}
	}
}


/* Function: FillMatZero
 * Description: Fills the matrix matGradC
 * Input: None
 * Output: None
 */
void FillMatZero()
{
	for(int index = 0; index < (pSystemObj->numConstraints * pSystemObj->numConstraints); index++)
	{
		pSystemObj->matZero[index] = +0.0;	// Fill the matrix with zeroes
	}
}


/* Function: FillMatGradCDot
 * Description: Fills the matrix matGradCDot
 * Input: None
 * Output: None
 */
void FillMatGradCDot()
{
	int row = 0, col = 0;
	int index = 0;
	double x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;
	int startFill = 0, endFill = 0;
	
	for(row = 0; row < (pSystemObj->numConstraints-2); row++)
	{
		startFill = -1;
		endFill = -1;

		for(col = 0; col < pSystemObj->degOfFreedom; col++)
		{
			if( col == (row * 2))
			{
				// Start of the blocks in the current row
				startFill = col;
				endFill = col+3;

				index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);
					
				x1 = pSystemObj->stateVelocity[col];
				y1 = pSystemObj->stateVelocity[col+1];
				x2 = pSystemObj->stateVelocity[col+2];
				y2 = pSystemObj->stateVelocity[col+3];

				pSystemObj->matGradCDot[index] = CDotDerivatives(x1, x2, CDOTDER1);
				pSystemObj->matGradCDot[index+1] = CDotDerivatives(y1, y2, CDOTDER2);
				pSystemObj->matGradCDot[index+2] = CDotDerivatives(x1, x2, CDOTDER3);
				pSystemObj->matGradCDot[index+3] = CDotDerivatives(y1, y2, CDOTDER4);
			}
			else	// Fill with zeroes in all other positions of the matrix current row
			{
				// if any of the other 3 valid blocks then dont fill with zeroes
				if((col >= startFill) && (col <= endFill))
				{
					continue;		
				}
				else	// You are all set to fill with zeroes :)
				{
					index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);
					pSystemObj->matGradCDot[index] = 0.0;
				}
			}

			// Make -Zero to + zero
			index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);
			if(pSystemObj->matGradCDot[index] == -0.0)
			{
				pSystemObj->matGradCDot[index] = +0.0;
			}

		} // for col
	} // for row

	// PIN CONSTRAINT FOR THE FIRST PARTICLE (RIGID FORM)
	for(col = 0; col < pSystemObj->degOfFreedom; col++)
	{
		index = FindIndexInMatrix(pSystemObj->numConstraints-2, col, pSystemObj->degOfFreedom);
		pSystemObj->matGradCDot[index] = +0.0;
	}

	index = FindIndexInMatrix(pSystemObj->numConstraints-2, 0, pSystemObj->degOfFreedom);
	x2 = pSystemObj->stateVelocity[0];
	y2 = pSystemObj->stateVelocity[1];
	
	pSystemObj->matGradCDot[index] = CDotDerivatives(PINX, x2, CDOTDER3);
	pSystemObj->matGradCDot[index+1] = CDotDerivatives(PINY, y2, CDOTDER4);

	// RING CONSTRAINT FOR THE LAST PARTICLE
	for(col = 0; col < pSystemObj->degOfFreedom; col++)
	{
		index = FindIndexInMatrix(pSystemObj->numConstraints-1, col, pSystemObj->degOfFreedom);
		pSystemObj->matGradCDot[index] = +0.0;
	}
	
	if(RING)
	{
		index = FindIndexInMatrix(pSystemObj->numConstraints-1, pSystemObj->degOfFreedom-2, pSystemObj->degOfFreedom);
		x1 = pSystemObj->stateVelocity[pSystemObj->degOfFreedom-2];		// Nth x
		y1 = pSystemObj->stateVelocity[pSystemObj->degOfFreedom-1];		// Nth y

		pSystemObj->matGradCDot[index] = CDotDerivatives(x1, 0, CDOTDERRINGX);
		pSystemObj->matGradCDot[index+1] = CDotDerivatives(y1, 0, CDOTDERRINGY);
	}
}


/* Function: FillMatCDotQDot
 * Description: Fills the matrix matCDotQDot mx1
 * Input: None
 * Output: None
 */
void FillMatCDotQDot()
{
	int row = 0, col = 0, index = 0;
	double rowScalar = 0.0;
	
	for(row = 0; row < pSystemObj->numConstraints; row++)
	{	
		rowScalar = 0.0;
		for(col = 0; col < pSystemObj->degOfFreedom; col++)
		{
			index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);
			rowScalar += pSystemObj->matGradCDot[index] * pSystemObj->stateVelocity[col];
		}
		pSystemObj->matCDotQDot[row] = -(rowScalar);

		// Make -Zero to + zero
		if(pSystemObj->matCDotQDot[row] == -0.0)
		{
			pSystemObj->matCDotQDot[row] = +0.0;
		}
	}
}


/* Function: FillMatCQDot
 * Description: Fills the matrix matCQDot mx1
 * Input: None
 * Output: None
 */
void FillMatCQDot()
{
	int row = 0, col = 0, index = 0;
	double rowScalar = 0.0;
	
	for(row = 0; row < pSystemObj->numConstraints; row++)
	{	
		rowScalar = 0.0;
		for(col = 0; col < pSystemObj->degOfFreedom; col++)
		{
			index = FindIndexInMatrix(row, col, pSystemObj->degOfFreedom);
			rowScalar += pSystemObj->matGradC[index] * pSystemObj->stateVelocity[col];
		}
		pSystemObj->matCQDot[row] = (rowScalar);

		// Make -Zero to + zero
		if(pSystemObj->matCQDot[row] == -0.0)
		{
			pSystemObj->matCQDot[row] = +0.0;
		}
	}
}


/* Function: FillMatFExt
 * Description: Fills the matrix matFExt nx1
 * Input: None
 * Output: None
 */
void FillMatFExt()
{
	int col = 0;

	for(col = 0; col < pSystemObj->degOfFreedom; col+=2)
	{
		pSystemObj->matFExt[col] = pSystemObj->pMass * PULLX;
		pSystemObj->matFExt[col+1] = pSystemObj->pMass * -GRAVITY;

		if(UNDERWATER)
		{
			pSystemObj->matFExt[col] += -(UNDERWATER_ALPHA * pSystemObj->stateVelocity[col]);
			pSystemObj->matFExt[col+1] += -(UNDERWATER_ALPHA * pSystemObj->stateVelocity[col+1]);
		}

	}
}


/* Function: FindXSqrYSqr
 * Description: Computes the (x2-x1)^2 OR (y2-y1)^2
 * Input: x1ORy1 - x1 or y1
 *		  x2ORy2 - x2 or y2	
 * Output: None
 */
double FindXSqrYSqr(double x1ORy1, double x2ORy2)
{
		double result = (pow((x2ORy2-x1ORy1), 2));

		return result;
}


/* Function: FillC
 * Description: Fills matrix C for computing the damping
 * Input: None
 * Output: None
 */
void FillC()
{
	int i = 0, j = 0, row = 0, col = 0;
	double x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0, xn = 0.0, yn = 0.0;
	
	// COMPUTE ALL THE RIGID CONSTRAINTS AND STORE IT IN C VECTOR
	for(row = 0; row < (pSystemObj->numConstraints-2); row++)
	{
		for(col = 0; col < pSystemObj->degOfFreedom; col++)
		{
			if( col == (row * 2))
			{
					
				x1 = pSystemObj->stateVector[col];
				y1 = pSystemObj->stateVector[col+1];
				x2 = pSystemObj->stateVector[col+2];
				y2 = pSystemObj->stateVector[col+3];
				
				// (x2 - x1)sqr + (y2 - y1)sqr - l sqr = 0
				pSystemObj->C[row] = FindXSqrYSqr(x1, x2) + FindXSqrYSqr(y1, y2) - (pow(pSystemObj->linkLength, 2));
			
				if(pSystemObj->C[row] == -0.0)
					pSystemObj->C[row] = +0.0;
			}
		}
	}
	
	// COMPUTE  PIN CONSTRAINT AND STORE IT IN C VECTOR
	// (x1) sqr + (y1) sqr - l sqr = 0
	x1 = pSystemObj->stateVector[0];
	y1 = pSystemObj->stateVector[1];
	pSystemObj->C[pSystemObj->numConstraints-2] = FindXSqrYSqr(0.0, x1) + FindXSqrYSqr(0.0, y1) - (pow(pSystemObj->linkLength, 2));
	
	if(pSystemObj->C[pSystemObj->numConstraints-2] == -0.0)
		pSystemObj->C[pSystemObj->numConstraints-2] = +0.0;

	if(RING)
	{
		// xn = DOF[n-2] yn = DOF[n-1]
		xn = pSystemObj->stateVector[pSystemObj->degOfFreedom-2];
		yn = pSystemObj->stateVector[pSystemObj->degOfFreedom-1];
		pSystemObj->C[pSystemObj->numConstraints-1] = FindXSqrYSqr(pRINGX, xn) + FindXSqrYSqr(pRINGY, yn) - pow((-pRINGY), 2);
	}

	if(pSystemObj->C[pSystemObj->numConstraints-1] == -0.0)
		pSystemObj->C[pSystemObj->numConstraints-1] = +0.0;
}


/* Function: DampIt
 * Description: Damp the forces which are making the chain lose the constraint Force
 * Input: None
 * Output: None
 */
void DampIt()
{
	int i = 0;
	
	FillMatCQDot();
	FillC();

	// Update the CDotQDot matrix with the damping value
	for(i = 0; i < pSystemObj->numConstraints; i++)
	{
		pSystemObj->matCDotQDot[i] += -(ALPHA * pSystemObj->matCQDot[i]) - (BETA * pSystemObj->C[i]);
	}
}


/* Function: CreateMatA
 * Description: creates the matrix A by combining M , \/C , \/CT , 0
 * Input: None
 * Output: None
 */
void CreateMatA()
{
	int index = 0, row = 0, col = 0, i = 0;

	// Storing matrix M
	i = -1;
	for(row = 0; row <= (pSystemObj->degOfFreedom-1); row++)
	{	
		for(col = 0; col <= (pSystemObj->degOfFreedom-1); col++)
		{
			i++;
			if(pSystemObj->matM[i] == -0.0)
			{
				pSystemObj->A[row][col] = +0.0;
			}
			else
			{
				pSystemObj->A[row][col] = pSystemObj->matM[i];
			}
		}
	}

	// Storing matrix \/CT
	i = -1;
	for(row = 0; row <= pSystemObj->degOfFreedom-1; row++)
	{	
		for(col = (0 + pSystemObj->degOfFreedom); col <= (pSystemObj->degOfFreedom + (pSystemObj->numConstraints-1)); col++)
		{
			i++;
			if(pSystemObj->matGradCTrans[i] == -0.0)
			{
				pSystemObj->A[row][col] = +0.0;
			}
			else
			{
				pSystemObj->A[row][col] = pSystemObj->matGradCTrans[i];				
			}
		}
	}

	// Storing matrix \/C
	i = -1;
	for(row = (0 + pSystemObj->degOfFreedom); row <= (pSystemObj->degOfFreedom + (pSystemObj->numConstraints-1)); row++)
	{	
		for(col = 0; col <= (pSystemObj->degOfFreedom-1); col++)
		{
			i++;
			if(pSystemObj->matGradC[i] == -0.0)
			{
				pSystemObj->A[row][col] = +0.0;
			}
			else
			{
				pSystemObj->A[row][col] = pSystemObj->matGradC[i];
			}
		}
	}

	// Storing matrix ZERO
	i = -1;
	for(row = (0 + pSystemObj->degOfFreedom); row <= (pSystemObj->degOfFreedom + (pSystemObj->numConstraints-1)); row++)
	{	
		for(col = (0 + pSystemObj->degOfFreedom); col <= (pSystemObj->degOfFreedom + (pSystemObj->numConstraints-1)); col++)
		{
			i++;
			if(pSystemObj->matZero[i] == -0.0)
			{
				pSystemObj->A[row][col] = +0.0;
			}
			else
			{
				pSystemObj->A[row][col] = pSystemObj->matZero[i];				
			}
		}
	}
}


/* Function: CreateMatB
 * Description: creates the matrix B by combining FExt and CDotQDot
 * Input: None
 * Output: None
 */
void CreateMatB()
{
	int i = 0, row = 0;
	
	// Storing matrix FExt 
	i = -1;
	for(row = 0; row <= (pSystemObj->degOfFreedom-1); row++)
	{
		i++;
		if(pSystemObj->matFExt[i] == -0.0)
		{
			pSystemObj->B[row] = +0.0;
		}
		else
		{
			pSystemObj->B[row] = pSystemObj->matFExt[i];
		}
	}

	// Storing matrix CDotQDot
	i = -1;
	for(row = (0 + pSystemObj->degOfFreedom); row <= (pSystemObj->degOfFreedom + (pSystemObj->numConstraints-1)); row++)
	{	
		i++;
		if(pSystemObj->matCDotQDot[i] == -0.0)
		{
			pSystemObj->B[row] = +0.0;
		}
		else
		{
			pSystemObj->B[row] = pSystemObj->matCDotQDot[i];
		}
	}
}


/* Function: computeAcceleration
 * Description: Computes the accleration by using SVD (AxX = B)
 * Input: None
 * Output: None
 */
void computeAcceleration()
{
	
	memset(pSystemObj->matM, 0, sizeof(double) * pSystemObj->degOfFreedom*pSystemObj->degOfFreedom);
	memset(pSystemObj->matGradC, 0, sizeof(double) * pSystemObj->numConstraints*pSystemObj->degOfFreedom);
	memset(pSystemObj->matGradCDot, 0, sizeof(double) * pSystemObj->numConstraints*pSystemObj->degOfFreedom);
	memset(pSystemObj->matGradCDot, 0, sizeof(double) * pSystemObj->numConstraints*pSystemObj->degOfFreedom);
	memset(pSystemObj->matGradCTrans, 0, sizeof(double) * pSystemObj->degOfFreedom*pSystemObj->numConstraints);
	memset(pSystemObj->matZero, 0, sizeof(double) * pSystemObj->numConstraints*pSystemObj->numConstraints);
	memset(pSystemObj->matCDotQDot, 0, sizeof(double) * pSystemObj->numConstraints);
	memset(pSystemObj->matCQDot, 0, sizeof(double) * pSystemObj->numConstraints);
	memset(pSystemObj->B, 0, sizeof(double) * pSystemObj->rowcolA);
	memset(pSystemObj->C, 0, sizeof(double) * pSystemObj->numConstraints);
	memset(pSystemObj->matFExt, 0, sizeof(double) * pSystemObj->degOfFreedom);
	memset(pSystemObj->matAcc, 0, sizeof(double) * pSystemObj->degOfFreedom);

	//------------CREATE MATRIX A-------
	FillMatM();
	FillMatGradC();
	FillMatZero();
	FillMatGradCTrans();
	CreateMatA();
	//----------------------------------

	//------------CREATE MATRIX B-------
	FillMatFExt();
	FillMatGradCDot();
	FillMatCDotQDot();
	DampIt();
	CreateMatB();
	//----------------------------------

	if(CREATEFILE)
	{
		if(stop <=1000)
		{
			PrintMat();
			stop++;
		}
		else if(stop == 1001)
		{
			printf("Done\n");
		}
	}

	SVDSolver();
}


/* Performs one step of Euler Integration 
   as a result, updates the Particle System Object */
void Euler()
{
  int col = 0;

  for(col = 0; col < pSystemObj->degOfFreedom; col++)
  {
		pSystemObj->matFExt[col] = 0.0;
		pSystemObj->matAcc[col] = 0.0;
  }

  // Fill A B and Find X and Extract Q Dot Dot from X. Fill matAcc from Q Dot Dot
  computeAcceleration();
  
  for (col = 0; col < pSystemObj->degOfFreedom; col += 2)
  {

	  // Velocity X Update
		pSystemObj->stateVelocity[col] += pSystemObj->h * pSystemObj->matAcc[col];
		// Velocity Y Update
		pSystemObj->stateVelocity[col+1] += pSystemObj->h * pSystemObj->matAcc[col+1];

		// Position X Update
		pSystemObj->stateVector[col] += pSystemObj->h * pSystemObj->stateVelocity[col];
		// Position Y Update
		pSystemObj->stateVector[col+1] += pSystemObj->h * pSystemObj->stateVelocity[col+1];

		
  }
}



/* THE GREAT SVD SOLVER - here it is */
void SVDSolver()
{
	int i = 0, j = 0;
	int decompRetVal = 0, solveRetVal = 0;
	double value = 0.0;

	// Matrix A
	gsl_matrix * A = gsl_matrix_alloc(pSystemObj->rowcolA, pSystemObj->rowcolA);
       
	for (i = 0; i < pSystemObj->rowcolA; i++)
	 for (j = 0; j < pSystemObj->rowcolA; j++)
	 {
		 value = (pSystemObj->A[i][j]);
		 gsl_matrix_set(A, i, j, value);
	 }

	// Matrix V
	gsl_matrix * V = gsl_matrix_alloc(pSystemObj->rowcolA, pSystemObj->rowcolA);
       
	for (i = 0; i < pSystemObj->rowcolA; i++)
	 for (j = 0; j < pSystemObj->rowcolA; j++)
		 gsl_matrix_set(V, i, j, 0.0);

	// Vector S
	gsl_vector * S = gsl_vector_alloc(pSystemObj->rowcolA);
       
	for (i = 0; i < pSystemObj->rowcolA; i++)
		gsl_vector_set (S, i, 0.0);

	// Vector work
	gsl_vector * work = gsl_vector_alloc(pSystemObj->rowcolA);
       
	for (i = 0; i < pSystemObj->rowcolA; i++)
		gsl_vector_set (work, i, 0.0);

	// Vector x
	gsl_vector * x = gsl_vector_alloc(pSystemObj->rowcolA);
       
	for (i = 0; i < pSystemObj->rowcolA; i++)
		gsl_vector_set (x, i, 0.0);

	// Vector b
	gsl_vector * b = gsl_vector_alloc(pSystemObj->rowcolA);
       
	for (i = 0; i < pSystemObj->rowcolA; i++)
		gsl_vector_set (b, i, pSystemObj->B[i]);

	// Decompose the Matrix A into U V S
	decompRetVal = gsl_linalg_SV_decomp (A, V, S, work);

	// Solve SVD to get the vector x
	solveRetVal = gsl_linalg_SV_solve(A, V, S, b, x);

	// Fetch the Acceleration part out of x
	for (i = 0; i < pSystemObj->degOfFreedom; i++)
		pSystemObj->matAcc[i] = gsl_vector_get(x, i);
}