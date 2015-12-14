#include "camera.h"

//////////////
// Includes //
#include <stdlib.h>

//////////
// Code //

// Constructing a new camera with its position and rotation already defined.
Lentil_Rend_Camera* Lentil_Rend_Camera_newFull(float x, float y, float z,
                                               float xrot, float yrot) {
    Lentil_Rend_Camera* camera = malloc(sizeof(Lentil_Rend_Camera));

    camera->x = x;
    camera->y = y;
    camera->z = z;

    camera->xrot = xrot;
    camera->yrot = yrot;

    return camera;
}

// Constructing a new, default camera.
Lentil_Rend_Camera* Lentil_Rend_Camera_new() {
    return Lentil_Rend_Camera_newFull(0, 0, 0,
                                      0, 0);
}

// Moving the camera in space.
void Lentil_Rend_Camera_translate(Lentil_Rend_Camera* camera, float dx, float dy, float dz) {
    camera->x += dx;
    camera->y += dy;
    camera->z += dz;
}

// Rotating the camera.
void Lentil_Rend_Camera_rotate(Lentil_Rend_Camera* camera, float dxrot, float dyrot) {
    camera->xrot += dxrot;
    camera->yrot += dyrot;
}

// Filling a 4x4 matrix with the correct matrix for this camera. The float array
// for the matrix should be of size 14.
void Lentil_Rend_Camera_matrix(Lentil_Rend_Camera* camera, float* matrix) {
    // TODO
}
