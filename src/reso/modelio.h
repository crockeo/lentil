#ifndef _LENTIL_RESO_MODELLOAD_H_
#define _LENTIL_RESO_MODELLOAD_H_

//////////////
// Includes //
#include "../core/errors.h"
#include "model.h"

//////////
// Code //

// Determining whether or not a given input / output format is valid.
bool Lentil_Reso_isValidFormat(const char*);

// Loading a model from a given location on disk with a given file format.
//
// Valid file formats:
//   obj  - The generic Wavefront object format.
//   lent - A more compressed file format made for this library.
void Lentil_Reso_loadModel(const char*, const char*, Lentil_Reso_Model*, Lentil_Core_Error*);

// Saving a model to a given location on disk with a given file format.
//
// Valid file formats:
//   obj  - The generic Wavefront object format.
//   lent - A more compressed file format made for this library.
void Lentil_Reso_saveModel(const char*, const char*, Lentil_Reso_Model*, Lentil_Core_Error*);

#endif
