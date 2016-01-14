#include "angles.h"

//////////
// Code //

// Converting n radians to n degrees.
float Lentil_Math_toDegree(float radian) {
    return radian * (180.f / M_PI);
}

// Converting n degrees to n radians.
float Lentil_Math_toRadian(float degree) {
    return degree * (M_PI / 180.f);
}
