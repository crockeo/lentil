#ifndef _LENTIL_MATH_VEC_H_
#define _LENTIL_MATH_VEC_H_

//////////
// Code //

// A general vector type.
typedef struct {
    int kind;
    float x, y, z, w;
} Lentil_Math_VecT;

const Lentil_Math_VecT ERROR_VEC = {-1, 0, 0, 0, 0};

// Constructing vectors of varying length.
Lentil_Math_VecT Lentil_Math_vec2(float, float);
Lentil_Math_VecT Lentil_Math_vec3(float, float, float);
Lentil_Math_VecT Lentil_Math_vec4(float, float, float, float);

// Basic vector operations.
Lentil_Math_VecT Lentil_Math_VecT_neg(Lentil_Math_VecT);
Lentil_Math_VecT Lentil_Math_VecT_inv(Lentil_Math_VecT);
Lentil_Math_VecT Lentil_Math_VecT_add(Lentil_Math_VecT, Lentil_Math_VecT);
Lentil_Math_VecT Lentil_Math_VecT_sub(Lentil_Math_VecT, Lentil_Math_VecT);
Lentil_Math_VecT Lentil_Math_VecT_mul(Lentil_Math_VecT, Lentil_Math_VecT);
Lentil_Math_VecT Lentil_Math_VecT_div(Lentil_Math_VecT, Lentil_Math_VecT);

#endif
