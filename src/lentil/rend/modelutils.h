#ifndef _LENTIL_REND_MODELUTILS_H_
#define _LENTIL_REND_MODELUTILS_H_

//////////////
// Includes //
#include <GL/glew.h>

#include "../core/errors.h"
#include "../reso/model.h"
#include "camera.h"

//////////
// Code //

#ifdef __cplusplus
extern "C" {
#endif

// A type to store the relevant bindings to vertex arrays / buffers for a given
// model render.
typedef struct Lentil_Rend_ModelRender {
    Lentil_Rend_Camera* camera;

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

#ifdef __cplusplus
}
#endif

#endif
