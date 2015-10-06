#ifndef _LENTIL_RESO_MODEL_H_
#define _LENTIL_RESO_MODEL_H_

//////////////
// Includes //
#include "../core/errors.h"

//////////
// Code //

// Information to represent a model resource.
typedef struct {

} Lentil_Reso_Model;

// Constructing a new Lentil_Reso_Model on the heap.
Lentil_Reso_Model* createModel();

// Freeing up all of the memory from a given model.
void Lentil_Reso_destroyModel(Lentil_Reso_Model*);

// Loading a .obj file into a Lentil_Reso_Model.
void Lentil_Reso_loadObj(const char*, Lentil_Reso_Model*, Lentil_Core_Error*);

// Saving a Lentil_Reso_Model to a given location on disk in the format of a
// .obj.
void Lentil_Reso_saveObj(const char*, Lentil_Reso_Model*, Lentil_Core_Error*);

// Loading a custom .lent file into a Lentil_Reso_Model. A .lent file is a
// custom file format to store 3D models in a smaller, more efficient form than
// a generic .obj file.
void Lentil_Reso_loadLent(const char*, Lentil_Reso_Model*, Lentil_Core_Error*);

// Saving a Lentil_Reso_Model to a given location on disk in the format of a
// .lent. Refer to the documentation for Lentil_Reso_loadLent(...) for what a
// .lent file is.
void Lentil_Reso_saveLent(const char*, Lentil_Reso_Model*, Lentil_Core_Error*);

#endif
