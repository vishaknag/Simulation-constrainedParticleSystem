/* Source: vector
 * Description: Contains vector mathematics functions.
 */

#include "vector.h"

/* Function: pDisp
 * Description: Displays the coordinates of a point
 * Input: p - Coordinates of a point
 * Output: Nothing
 */
void pDisp(point p)
{
	printf("Point: (%f, %f, %f)\n", p.x, p.y, p.z);
} //end pDisp

/* Function: dotProd
 * Description: Calculates the dot product of two 3D vectors
 * Input: v1 - Coordinates of the first vector
 *        v2 - Coordinates of the second vector
 * Output: returns the dot product results of the two input vectors
 */
double dotProd(point v1, point v2)
{
	return (double)(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
} //end dotProd

/* Function: vecLeng
 * Description: Calculates length of a vector betweem two points
 * Input: v1 - Coordinates of the first vector
 *        v2 - Coordinates of the second vector
 * Output: returns the length of the vector
 */
double vecLeng(point v1, point v2)
{
	point dest;

	pDIFFERENCE(v1, v2, dest);

	return sqrt(dest.x * dest.x + dest.y * dest.y + dest.z * dest.z);
} //end vecLeng