#ifndef _LENTIL_REND_MATRIXGEN_H_
#define _LENTIL_REND_MATRIXGEN_H_

//////////
// Code //

#ifdef __cplusplus
extern "C" {
#endif

// Filling a 3x3 double matrix with the appropriate values for a given rotation
// about the y and x axes respectively.
//
// That is to say, we're rotating along the 2D plane defined along (x, z), and
// the 2D plane along (y, z).
void Lentil_Rend_fillMatrix(float*, float, float);

#ifdef __cplusplus
}
#endif

#endif
