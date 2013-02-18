/* Header: physics
 * Description: Header file for physics calculations
 */

#ifndef _PHYSICS_H_
#define _PHYSICS_H_


#define NSTEP 4
#define VELOCITY 0

extern int PCOUNT;
extern double pRINGX, pRINGY;
extern int  UNDERWATER;
extern double  GRAVITYFORCE;
extern double PULLX;
extern float UNDERWATER_ALPHA;
extern float TIMESTEP;
extern float BETA;
extern float GRAVITY;
extern int RUNTEST;

class particleSystem
{
public:
		int n;						// display every nth timepoint  
		int pCount;					// Count of the number of particles present in the simulation
		int degOfFreedom;			// Each particle has two degrees of freedom
		int numConstraints;			// Number of scalar equations
		int rowcolA;				// size of the row/col in matrix A

		double pMass;				// Mass of each particle in the particle chain
		double linkLength;			// Length of the connecting link between two particles
		double Kd;					// Damping Constant
		double Ks;					// Stiffness or Hooks Constant
		double h;					// timestep
		
		point *pPosition;			// current positions of each particle in the chain

		double *matM;				// Matrix of masses of all the particles - M
		double *C;					// Vector of Constraints
		double *matGradC;			// Gradient matrix of Constraint vector diff wrt state vector - \/C
		double *matGradCTrans;		// Transpose of the Gradient of Constraint matrix - \/CT
		double *matGradCDot;		// Gradient/Jacobian matrix of Constraint vector Dot - \/CDot
		double *matZero;			// Zero matrix which fills the matrix A in AX = B matrix equation - 0
		double **A;					// Matrrix A consists of mat M, \/C, \/CT, zero
		double *B;					// Matrix B consists of mat FExt and mat CDotQDot
		double *stateVector;		// Stores the current state of the particles
		double *stateVelocity;		// Stores the current velocity of the particles
		double *matCDotQDot;		// Gradient Dot Cross velocity vector (mxn * nx1) = mx1
		double *matCQDot;			// Gradient Cross velocity vector (mxn * nx1) = mx1
		double *matFExt;			// FExt force due to Gravity and user input
		double *matAcc;				// Acceleration due to Gravity and user input

		particleSystem(int numParticles);
};
extern particleSystem *pSystemObj;

//------------CREATE MATRIX A-------
void FillMatM();
void FillMatGradC();
void FillMatZero();
void FillMatGradCTrans();
void CreateMatA();
	//----------------------------------

	//------------CREATE MATRIX B-------
void FillMatFExt();
void FillMatGradCDot();
void FillMatCDotQDot();
void DampIt();
void CreateMatB();
//----------------------------------

//void WriteToFile();

double FindXSqrYSqr(double x1ORy1, double x2ORy2);
void PrintMat();
void Euler();
void SVDSolver();

void CreateFile();
void OpenFile();
void CloseFile();

void ParticleSystemInit();
int FindIndexInMatrix(int row, int column, int sizeOfRow);

void GetCircumferencePoints(double Radius, double OriginX, double OriginY, double Angle, int index);
#endif
