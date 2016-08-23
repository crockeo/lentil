#ifndef _LENTIL_RESO_MATERIAL_H_
#define _LENTIL_RESO_MATERIAL_H_

//////////////
// Includes //
#include <stdio.h>

#include "../core/errors.h"

//////////
// Code //

#ifdef __cplusplus
extern "C" {
#endif

// A single material defined within an .mtl file by newmtl statements.
typedef struct {
    const char* name;
} Lentil_Reso_Material;

// Creating a new, empty Lentil_Reso_Material with a given name.
Lentil_Reso_Material* Lentil_Reso_Material_new(const char* name);

// A set of materials as defined by a .mtl file.
typedef struct {
    int materialsSize, materialsLength;
    Lentil_Reso_Material** materials;
} Lentil_Reso_MaterialSet;

// Creating a new, empty Lentil_Reso_MaterialSet.
Lentil_Reso_MaterialSet* Lentil_Reso_MaterialSet_new();

// Destroying a Lentil_Reso_MaterialSet and its subsidiary material count.
void Lentil_Reso_MaterialSet_destroy(Lentil_Reso_MaterialSet*);

// Loading a Wavefront .mtl file into a Lentil_Reso_Material* for use with
// object files.
void Lentil_Rest_loadMaterial(FILE*, Lentil_Reso_MaterialSet*, Lentil_Core_Error*);

// Loading a Wavefront .mtl file into a Lentil_Reso_Material* for use with
// object files, but using a file path rather than a file handle.
void Lentil_Reso_loadMaterialStr(const char*, Lentil_Reso_MaterialSet*, Lentil_Core_Error*);

#ifdef __cplusplus
}
#endif

#endif
