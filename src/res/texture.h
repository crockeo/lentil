#ifndef _LENTIL_RESO_TEXTURE_H_
#define _LENTIL_RESO_TEXTURE_H_

//////////////
// Includes //
#include <GL/glew.h>
#include <stdio.h>

#include "../errors.h"

//////////
// Code //

// Populating a given texture (represented as a GLuint) with some PNG data
// (represented by a FILE pointer).
void Lentil_Reso_populateTexture(FILE*, GLuint, Lentil_Core_Error*);

// Attempting to load a texture from a given location on disk.
//
// The texture must be in a .png format.
GLuint Lentil_Reso_loadTexture(const char*, Lentil_Core_Error*);

#endif
