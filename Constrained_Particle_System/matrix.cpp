/* Source: matrix
 * Description: Contains functions that compute matrix calculations
 */

#include "render.h"
#include "matrix.h"

/* Function: matMult31
 * Description: Computes matrix multiplication of 3x1 * 1x3
 * Input: m1 - 3x1 input matrix
		  m2 - 1x3 input matrix
		  mat - resulting 3x3 matrix pointer
 * Output: None
 */
void matMult31(point m1, point m2, matrix33 *mat)
{
	*mat[0][0] = m1.x * m2.x;
	*mat[0][1] = m1.x * m2.y;
	*mat[0][2] = m1.x * m2.y;

	*mat[1][0] = m1.y * m2.x;
	*mat[1][1] = m1.y * m2.y;
	*mat[1][2] = m1.y * m2.z;

	*mat[2][0] = m1.z * m2.x;
	*mat[2][1] = m1.z * m2.y;
	*mat[2][2] = m1.z * m2.z;
}


/* Function: matSqrt33
 * Description: Computes the Square Root of a 3x3 matrix
 * Input: m1 - 3x3 input matrix
 *        mat - resulting Square Root of the 3x3 matrix
 * Output: None
 */
void matMult3331(matrix33 m1, point v1, point *vec)
{
	(*vec).x = v1.x * m1[0][0] + v1.y * m1[0][1] + v1.z * m1[0][2];
	(*vec).y = v1.x * m1[1][0] + v1.y * m1[1][1] + v1.z * m1[1][2];
	(*vec).z = v1.x * m1[2][0] + v1.y * m1[2][1] + v1.z * m1[2][2];
}


/* Function: matScalarMult33
 * Description: Computes scalar multiplication of a 3x3
 * Input: scalar - scalar value
 *        m1 - 3x3 input matrix
 *        mat - resulting 3x3 matrix pointer
 * Output: None
 */
void matScalarMult33(double scalar, matrix33 m1, matrix33 *mat)
{
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++)
			*mat[row][col] = scalar * m1[row][col];
}


/* Function: matAdd33
 * Description: Computes addition of 3x3 matrices
 * Input: m1 - 3x3 input matrix
 *        m2 - 3x3 input matrix
 *        mat - resulting 3x3 matrix pointer
 * Output: None
 */
void matAdd33(matrix33 m1, matrix33 m2, matrix33 *mat)
{
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++)
			*mat[row][col] = m1[row][col] + m2[row][col];
}


/* Function: matTranspose33
 * Description: Computes the transpose of a 3x3 matrix
 * Input: m1 - 3x3 input matrix
 *        mat - resulting transpose of the 3x3 matrix
 * Output: None
 */
void matTranspose33(matrix33 m1, matrix33 *mat)
{
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++)
			*mat[col][row] = m1[row][col];
}


/* Function: matDeterminant33
 * Description: Computes the Determinant of a 3x3 matrix
 * Input: m1 - 3x3 input matrix
 *        mat - resulting Determinant of the 3x3 matrix
 * Output: None
 */
double matDeterminant33(matrix33 m)
{
	double term1 = 0, term2 = 0, term3 = 0, result = 0;

	term1 = m[0][0] * (m[1][1]*m[2][2] - m[1][2]*m[2][1]);
	term2 = m[0][1] * (m[1][2]*m[2][0] - m[2][2]*m[1][0]);
	term3 = m[0][2] * (m[1][0]*m[2][1] - m[1][1]*m[2][0]);

	result = term1+term2+term3;

	return result;
}


/* Function: matInverse33
 * Description: Computes the Inverse of a 3x3 matrix
 * Input: m1 - 3x3 input matrix
 *        mat - resulting Inverse of the 3x3 matrix
 * Output: None
 */
void matInverse33(matrix33 m1, matrix33 *mat)
{
	double detM = 0;
	matrix33 myMat;

	detM = 1.0 / matDeterminant33(m1);
	matTranspose33(m1, &myMat);
	matScalarMult33(detM, myMat, mat);
}


/* Function: matMult33
 * Description: Computes the Multiplication of two 3x3 matrices
 * Input: m1 - 3x3 input matrix
 *		  m2 - 3x3 input matrix
 *        mat - resulting Product of two 3x3 matrices
 * Output: None
 */
void matMult33(matrix33 m1, matrix33 m2, matrix33 *mat)
{
	point curCol, curRow;

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			 curCol.x = m2[0][col];
			 curCol.y = m2[1][col];
			 curCol.z = m2[2][col];

			 curRow.x = m1[row][0];
			 curRow.y = m1[row][1];
			 curRow.z = m1[row][2];

			 *mat[row][col] = dotProd(curCol, curRow);
		}
	}
}


/* Function: matSqrt33
 * Description: Computes the Square Root of a 3x3 matrix
 * Input: m1 - 3x3 input matrix
 *        mat - resulting Square Root of the 3x3 matrix
 * Output: None
 */
void matSqrt33(matrix33 m1, matrix33 *mat)
{
	
}