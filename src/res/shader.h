#ifndef _SHADER_H_
#define _SHADER_H_

//////////////
// Includes //
#include <GL/glew.h>

#include "../errors.h"

//////////
// Code //

// Attempting to load a shader from a given location on disk.
//
// Upon failure it will populate Lentil_Core_Error with the appropriate
// information.
GLuint Lentil_loadShader(const char*, Lentil_Core_Error*);

#endif
