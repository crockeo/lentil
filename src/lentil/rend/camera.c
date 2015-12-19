#include "camera.h"

//////////////
// Includes //
#include <stdlib.h>
#include <math.h>

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

// Destroying a camera.
void Lentil_Rend_Camera_destroy(Lentil_Rend_Camera* camera) {
    free(camera);
}

// Moving the camera in space, with the axes shifted according to the rotation.
void Lentil_Rend_Camera_translateRot(Lentil_Rend_Camera* camera, float dx, float dy, float dz) {
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
    if (camera->xrot < 0)
        camera->xrot += 2 * M_PI;
    else if (camera->xrot > 2 * M_PI)
        camera->xrot -= 2 * M_PI;

    camera->yrot += dyrot;
    if (camera->yrot < 0)
        camera->yrot += 2 * M_PI;
    else if (camera->yrot > 2 * M_PI)
        camera->yrot -= 2 * M_PI;
}

// Filling a 4x4 matrix with the correct matrix for this camera. The float array
// for the matrix should be of size 14.
void Lentil_Rend_Camera_matrix(Lentil_Rend_Camera* camera, float* matrix) {
    // TODO
}
