/* Header: matrix
 * Description: Header file for matrix calculations
 */

#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef double matrix33[3][3];

void matMult31(point m1, point m2, matrix33 *mat);
void matMult3331(matrix33 m1, point v1, point *vec);
void matScalarMult33(double scalar, matrix33 m1, matrix33 *mat);
void matAdd33(matrix33 m1, matrix33 m2, matrix33 *mat);
void matTranspose33(matrix33 m1, matrix33 *mat);
double matDeterminant33(matrix33 m);
void matInverse33(matrix33 m1, matrix33 *mat);
void matMult33(matrix33 m1, matrix33 m2, matrix33 *mat);
void matSqrt33(matrix33 m1, matrix33 *mat);

#endif
