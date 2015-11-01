#ifndef _LENTIL_REND_MODELUTILS_H_
#define _LENTIL_REND_MODELUTILS_H_

//////////////
// Includes //
#include <GL/glew.h>

#include "../core/errors.h"
#include "../reso/model.h"

//////////
// Code //

#ifdef __cplusplus
extern "C" {
#endif

// Filling a VBO and EBO with the information from a single
// Lentil_Reso_Model_Group.
int Lentil_Rend_fillBuffers(Lentil_Reso_Model*, int, GLuint, GLuint, Lentil_Core_Error*);

// A type to store the relevant bindings to vertex arrays / buffers for a given
// model render.
typedef struct Lentil_Rend_ModelRender {
    float xrot, yrot;

    int count;
    int* vLengths;

    GLuint* vaos;
    GLuint* vbos;
    GLuint* ebos;
} Lentil_Rend_ModelRender;

// Constructing a new Lentil_Rend_ModelRender from a Lentil_Reso_Model.
Lentil_Rend_ModelRender* Lentil_Rend_ModelRender_new(Lentil_Reso_Model*, Lentil_Core_Error*);

// Destroying the data contained in a Lentil_Rend_ModelRender.
void Lentil_Rend_ModelRender_destroy(Lentil_Rend_ModelRender*);

// Performing a render upon the data contained within a Lentil_Rend_ModelRender.
void Lentil_Rend_ModelRender_render(Lentil_Rend_ModelRender*, GLuint, GLuint);

// Updating the rotation and then performing a render.
void Lentil_Rend_ModelRender_renderRot(Lentil_Rend_ModelRender*, GLuint, GLuint, float, float);

#ifdef __cplusplus
}
#endif

#endif
