#ifndef _LENTIL_REND_RENDERER_H_
#define _LENTIL_REND_RENDERER_H_

//////////////
// Includes //
#include <GL/glew.h>

#include "../core/errors.h"
#include "../reso/model.h"

//////////
// Code //

// Filling a VBO with the information from a model.
void Lentil_Rend_fillVBO(Lentil_Reso_Model*, GLuint, Lentil_Core_Error*);

// Filling an EBO with the information from a model.
void Lentil_Rend_fillEBO(Lentil_Reso_Model*, GLuint, Lentil_Core_Error*);

#endif
