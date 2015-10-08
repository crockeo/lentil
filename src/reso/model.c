#include "model.h"

//////////////
// Includes //
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../core/debug.h"

//////////
// Code //

// Constructing a new Lentil_Reso_Model on the heap.
Lentil_Reso_Model* Lentil_Reso_createModel() {
    Lentil_Reso_Model* model = malloc(sizeof(Lentil_Reso_Model));

    model->numVertices = 0;
    model->vertices = NULL;

    model->numTextureVertices = 0;
    model->textureVertices = 0;

    model->numNormalVertices = 0;
    model->normalVertices = 0;

    return model;
}

// Freeing up all of the memory from a given model.
void Lentil_Reso_destroyModel(Lentil_Reso_Model* model) {
    if (model->vertices != NULL)
        free(model->vertices);
    if (model->textureVertices != NULL)
        free(model->textureVertices);
    if (model->normalVertices != NULL)
        free(model->normalVertices);
}

// Determining whether or not a given input / output format is valid.
bool Lentil_Reso_isValidFormat(const char* format) {
    return strcmp(format, "obj") == 0 || strcmp(format, "lent") == 0;
}

// Loading a model from a given location on disk with a given file format.
//
// Valid file formats:
//   obj  - The generic Wavefront object format.
//   lent - A more compressed file format made for this library.
void Lentil_Reso_loadModel(const char* path, const char* format, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    if (!Lentil_Reso_isValidFormat(format)) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("modelload: path \"%s\" invalid format \"%s\".\n", path, format);

        pErr->code = Lentil_Core_INVALIDMODELFORMAT;
        return;
    }

    FILE* out = fopen(path, "w");
    if (out == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("modelload: error in opening file \"%s\"\n", path);

        pErr->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    if (strcmp(format, "obj") == 0) {
        // TODO: Load from an object.
    } else if (strcmp(format, "lent") == 0) {
        // TODO: Create a .lent file format.
    } else {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("modelload: path \"%s\" shouldn't pass \"%s\".\n", path, format);

        pErr->code = Lentil_Core_INVALIDMODELFORMAT;
        fclose(out);
        return;
    }

    fclose(out);
}

// Saving a model to a given location on disk with a given file format.
//
// Valid file formats:
//   obj  - The generic Wavefront object format.
//   lent - A more compressed file format made for this library.
void Lentil_Reso_saveModel(const char* path, const char* format, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    if (!Lentil_Reso_isValidFormat(format)) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("modelsave: path \"%s\" invalid format \"%s\".\n", path, format);

        pErr->code = Lentil_Core_INVALIDMODELFORMAT;
        return;
    }

    FILE* out = fopen(path, "w");
    if (out == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("modelsave: error in opening file \"%s\"\n", path);

        pErr->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    if (strcmp(format, "obj") == 0) {
        // TODO: Save to an object.
    } else if (strcmp(format, "lent") == 0) {
        // TODO: Create a .lent file format.
    } else {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("modelsave: path \"%s\" shouldn't pass \"%s\".\n", path, format);

        pErr->code = Lentil_Core_INVALIDMODELFORMAT;
        fclose(out);
        return;
    }

    fclose(out);
}
