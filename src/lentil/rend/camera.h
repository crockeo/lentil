#ifndef _CAMERA_H_
#define _CAMERA_H_

//////////
// Code //

#ifdef __cplusplus
extern "C" {
#endif

// A struct to represent a camera in 3d space.
typedef struct {
    // Position in space.
    float x, y, z;

    // Rotation of the camera.
    float xrot, yrot;
} Lentil_Rend_Camera;

// Constructing a new camera with its position and rotation already defined.
Lentil_Rend_Camera* Lentil_Rend_Camera_newFull(float, float, float, float, float);

// Constructing a new, default camera.
Lentil_Rend_Camera* Lentil_Rend_Camera_new();

// Destroying a camera.
void Lentil_Rend_Camera_destroy(Lentil_Rend_Camera*);

// Moving the camera in space.
void Lentil_Rend_Camera_translate(Lentil_Rend_Camera*, float, float, float);

// Rotating the camera.
void Lentil_Rend_Camera_rotate(Lentil_Rend_Camera*, float, float);

// Filling a 4x4 matrix with the correct matrix for this camera.
void Lentil_Rend_Camera_matrix(Lentil_Rend_Camera*, float*);

#ifdef __cplusplus
}
#endif

#endif
