#include "matrixgen.h"

//////////////
// Includes //
#include <stdio.h>
#include <math.h>

//////////
// Code //

#define asin(val) sin(val / (180 / M_PI))
#define acos(val) cos(val / (180 / M_PI))

// Filling a 3x3 double matrix with the appropriate values for a given rotation
// about the y and x axes respectively.
//
// That is to say, we're rotating along the 2D plane defined along (x, z), and
// the 2D plane along (y, z).
void Lentil_Rend_fillMatrix(float* matrix, float xrot, float yrot) {
    // TODO: Find out how to do a x-axis rotation and a y-axis rotation?

    matrix[0] =  acos(xrot); matrix[1] =        0.0; matrix[2] =  asin(xrot);
    matrix[3] =         0.0; matrix[4] = acos(yrot); matrix[5] = -asin(yrot);
    matrix[6] = -asin(xrot); matrix[7] = asin(yrot); matrix[8] =  acos(xrot) * acos(yrot);
}
