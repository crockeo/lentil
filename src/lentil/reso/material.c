#include "material.h"

//////////////
// Includes //
#include <stdlib.h>
#include <string.h>

#include "../core/array.h"
#include "file.h"

//////////
// Code //

// Creating a new, empty Lentil_Reso_Material with a given name.
Lentil_Reso_Material* Lentil_Reso_Material_new(const char* name) {
    // TODO: Create an empty material struct.
}

// Creating a new, empty Lentil_Reso_MaterialSet.
Lentil_Reso_MaterialSet* Lentil_Reso_MaterialSet_new() {
    Lentil_Reso_MaterialSet* materialSet = malloc(sizeof(Lentil_Reso_MaterialSet));
    Lentil_Core_initArray(materialSet->materials, Lentil_Reso_Material*);
    return materialSet;
}

// Destroying a Lentil_Reso_MaterialSet and its subsidiary material count.
void Lentil_Reso_MaterialSet_destroy(Lentil_Reso_MaterialSet* materialSet) {
    for (int i = 0; i < materialSet->materialsSize; i++)
        free(materialSet->materials[i]);
    free(materialSet->materials);
    free(materialSet);
}


// Loading a Wavefront .mtl file into a Lentil_Reso_Material* for use with
// object files.
void Lentil_Rest_loadMaterial(FILE* file, Lentil_Reso_MaterialSet* materialSet, Lentil_Core_Error* err) {
    char line[256];
    char c = '\0';
    int i;

    while (c != EOF) {
        for (i = 0; i < 256; i++) {
            c = fgetc(file);
            if (c == EOF || c == '\n' || c == '\r')
                break;
        }

        line[i] = '\0';
        char copy[256];
        memcpy(copy, line, 256);

        Lentil_Reso_Material* mat = Lentil_Reso_Material_new(copy);
        Lentil_Core_addElement(materialSet->materials, mat, Lentil_Reso_Material*);
    }
}

// Loading a Wavefront .mtl file into a Lentil_Reso_Material* for use with
// object files, but using a file path rather than a file handle.
void Lentil_Reso_loadMaterialStr(const char* path, Lentil_Reso_MaterialSet* materialSet, Lentil_Core_Error* err) {
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        err->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    Lentil_Rest_loadMaterial(f, materialSet, err);
}
