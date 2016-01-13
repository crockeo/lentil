#include "vec.h"

//////////
// Code //

// Constructing vectors of varying length.
Lentil_Math_VecT Lentil_Math_vec2(float x, float y) {
    return (Lentil_Math_VecT){2, x, y, 0, 1};
}

Lentil_Math_VecT Lentil_Math_vec3(float x, float y, float z) {
    return (Lentil_Math_VecT){3, x, y, z, 1};
}

Lentil_Math_VecT Lentil_Math_vec4(float x, float y, float z, float w) {
    return (Lentil_Math_VecT){4, x, y, z, w};
}

// Basic vector operations.
Lentil_Math_VecT Lentil_Math_VecT_neg(Lentil_Math_VecT vec) {
    return (Lentil_Math_VecT){vec.kind, -vec.x, -vec.y, -vec.z, vec.w};
}

Lentil_Math_VecT Lentil_Math_VecT_inv(Lentil_Math_VecT vec) {
    return (Lentil_Math_VecT){-1, 1 / vec.x, 1 / vec.y, 1 / vec.z, 1 / vec.w};
}

Lentil_Math_VecT Lentil_Math_VecT_add(Lentil_Math_VecT vec1, Lentil_Math_VecT vec2) {
    return (Lentil_Math_VecT) {
        vec1.kind == vec2.kind ? vec1.kind : -1,
        vec1.x + vec2.x,
        vec1.y + vec2.y,
        vec1.z + vec2.z,
        vec1.w + vec2.w
    };
}

Lentil_Math_VecT Lentil_Math_VecT_sub(Lentil_Math_VecT vec1, Lentil_Math_VecT vec2) {
    return Lentil_Math_VecT_add(vec1, Lentil_Math_VecT_neg(vec2));
}

Lentil_Math_VecT Lentil_Math_VecT_mul(Lentil_Math_VecT vec1, Lentil_Math_VecT vec2) {
    return (Lentil_Math_VecT) {
        vec1.kind == vec2.kind ? vec1.kind : -1,
        vec1.x * vec2.x,
        vec1.y * vec2.y,
        vec1.z * vec2.z,
        vec1.w * vec2.w
    };
}
Lentil_Math_VecT Lentil_Math_VecT_div(Lentil_Math_VecT vec1, Lentil_Math_VecT vec2) {
    return Lentil_Math_VecT_mul(vec1, Lentil_Math_VecT_neg(vec2));
}
