#ifndef _LENTIL_REND_BUFFER_H_
#define _LENTIL_REND_BUFFER_H_

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

// Using a Lentil_Rend_Model to fill a VAO's VBO and EBO with the appropriate
// vertices. The choice of buffer use is defined by the GLenum.
//
// This function returns the vertex count.
int Lentil_Rend_fillBuffers(Lentil_Reso_Model*, int, GLuint, GLuint, GLuint, GLenum, Lentil_Core_Error*);

#ifdef __cplusplus
}
#endif

#endif
