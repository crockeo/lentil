#ifndef _LENTIL_RESO_MODEL_H_
#define _LENTIL_RESO_MODEL_H_

//////////////
// Includes //
#include <stdbool.h>

#include "../core/errors.h"

//////////
// Code //

// Information to represent a model resource.
typedef struct {
    int numVertices;
    float* vertices;

    int numTextureVertices;
    float* textureVertices;

    int numNormalVertices;
    float* normalVertices;
} Lentil_Reso_Model;

// Constructing a new Lentil_Reso_Model on the heap.
Lentil_Reso_Model* Lentil_Reso_createModel();

// Freeing up all of the memory from a given model.
void Lentil_Reso_destroyModel(Lentil_Reso_Model*);

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
