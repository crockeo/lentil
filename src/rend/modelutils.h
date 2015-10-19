#ifndef _LENTIL_REND_MODELUTILS_H_
#define _LENTIL_REND_MODELUTILS_H_

//////////////
// Includes //
#include <GL/glew.h>

#include "../core/errors.h"
#include "../reso/model.h"

//////////
// Code //

// Filling a VBO and EBO with the information from a single
// Lentil_Reso_Model_Group.
int Lentil_Rend_fillBuffers(Lentil_Reso_Model*, int, GLuint, GLuint, Lentil_Core_Error*);

// Rendering a model with a given texture and a given shader.
void Lentil_Rend_renderModel(Lentil_Reso_Model*, GLuint, GLuint, Lentil_Core_Error*);

#endif
