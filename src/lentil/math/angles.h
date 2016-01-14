#ifndef _LENTIL_MATH_ANGLES_H_
#define _LENTIL_MATH_ANGLES_H_

//////////////
// Includes //
#include <math.h>

//////////
// Code //

////
// Making sure M_PI is available for everyone!
#ifndef M_PI
#define M_PI 3.1415926535
#endif

////
// Note:
//   Despite there being functions to convert 'toDegree's from radians, the
//   default measure of angle for the rest of the functions in this section
//   assume you're using radians.

// Converting n radians to n degrees.
float Lentil_Math_toDegree(float);

// Converting n degrees to n radians.
float Lentil_Math_toRadian(float);

#endif
