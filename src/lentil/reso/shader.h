#ifndef _LENTIL_RESO_SHADER_H_
#define _LENTIL_RESO_SHADER_H_

//////////////
// Includes //
#include <GL/glew.h>

#include "../core/errors.h"

//////////
// Code //

#ifdef __cplusplus
extern "C" {
#endif

// Attempting to load a shader from a given location on disk.
GLuint Lentil_Reso_loadShader(const char*, GLenum, Lentil_Core_Error*);

// Attempting to load a shader program from a given location on disk. It scans
// for shaders at the path + ".vert", ".geom", and ".frag" respective.
GLuint Lentil_Reso_loadShaderProgram(const char*, Lentil_Core_Error*);

#ifdef __cplusplus
}
#endif

#endif
