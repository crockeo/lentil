#include "model.h"

//////////////
// Includes //
#include <stdlib.h>

//////////
// Code //

// Constructing a new Lentil_Reso_Model on the heap.
Lentil_Reso_Model* createModel() {
    return NULL;
}

// Freeing up all of the memory from a given model.
void Lentil_Reso_destroyModel(Lentil_Reso_Model* model) {
}

// Loading a .obj file into a Lentil_Reso_Model.
void Lentil_Reso_loadObj(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {

}

// Saving a Lentil_Reso_Model to a given location on disk in the format of a
// .obj.
void Lentil_Reso_saveObj(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {

}

// Loading a custom .lent file into a Lentil_Reso_Model. A .lent file is a
// custom file format to store 3D models in a smaller, more efficient form than
// a generic .obj file.
void Lentil_Reso_loadLent(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {

}

// Saving a Lentil_Reso_Model to a given location on disk in the format of a
// .lent. Refer to the documentation for Lentil_Reso_loadLent(...) for what a
// .lent file is.
void Lentil_Reso_saveLent(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {

}
