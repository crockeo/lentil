#ifndef _LENTIL_RESO_OBJLOADER_H_
#define _LENTIL_RESO_OBJLOADER_H_

//////////////
// Includes //
#include <stdio.h>

#include "../core/errors.h"
#include "model.h"

//////////
// Code //

// Attempting to load a model from Wavefront .obj file. Lentil only supports
// a subset of .obj files:
//
// * Vertices         (v)
// * Texture vertices (vt)
// * Normal vertices  (vn)
// * Materials        (mtllib / usemtl)
// * Groups           (g)
// * Faces            (f)
void Lentil_Reso_loadObjModel(FILE*, Lentil_Reso_Model*, Lentil_Core_Error*);

// Effectivel the same thing as calling Lentil_Reso_loadObjModel(fopen(path)).
// Refer to the documentation of the above for use.
void Lentil_Reso_loadObjModelStr(const char*, Lentil_Reso_Model*, Lentil_Core_Error*);

#endif
