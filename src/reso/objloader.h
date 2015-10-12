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
void Lentil_Reso_Model_loadObj(FILE*, Lentil_Reso_Model*, Lentil_Core_Error*);

#endif
