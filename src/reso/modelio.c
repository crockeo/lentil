#include "modelio.h"

//////////////
// Includes //
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../core/debug.h"
#include "file.h"

//////////
// Code //

// Determining whether or not a given input / output format is valid.
bool Lentil_Reso_isValidFormat(const char* format) {
    return strcmp(format, "obj") == 0 || strcmp(format, "lent") == 0;
}

// Attempting to load a single face from a model file.
void Lentil_Reso_loadFace(FILE* modelFile, Lentil_Reso_Face* face, Lentil_Core_Error* pErr) {
    Lentil_Reso_consumeLine(modelFile);
}

// Loading a model from a .obj file.
void Lentil_Reso_loadModelObj(FILE* modelFile, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    int tokenSize = 64;
    char* token = malloc(tokenSize * sizeof(char));

    bool hasFace = false;
    while (!Lentil_Reso_loadToken(modelFile, token, tokenSize)) {
        int n;

        if (strcmp(token, "#") == 0 || strcmp(token, "g") == 0 || strcmp(token, "mtllib") == 0 || strcmp(token, "usemtl")
            Lentil_Reso_consumeLine(modelFile);
        else if (strcmp(token, "v") == 0) {
            float x, y, z, w;
            n = fscanf(modelFile, "%f %f %f %f", &x, &y, &z, &w);
            if (n < 3 || n > 4) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("Invalid vertex coordinate.\n");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                return;
            }

            if (n == 3)
                w = 1.f;

            Lentil_Reso_addPosVertex(model, x, y, z, w);
        } else if (strcmp(token, "vt") == 0) {
            float x, y, w;
            n = fscanf(modelFile, "%f %f %f", &x, &y, &w);
            if (n < 2 || n > 3) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("Invalid texture coordinate.\n");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                return;
            }

            if (n == 2)
                w = 0;

            Lentil_Reso_addTexVertex(model, x, y, w);
        } else if (strcmp(token, "vn") == 0) {
            float x, y, z;
            n = fscanf(modelFile, "%f %f %f", &x, &y, &z);
            if (n != 3) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("Invalid normal coordinate.\n");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                return;
            }

            Lentil_Reso_addNorVertex(model, x, y, z);
        } else if (strcmp(token, "f") == 0) {
            Lentil_Core_Error err;
            Lentil_Reso_Face face;
            Lentil_Reso_loadFace(modelFile, &face, &err);
            if (Lentil_Core_isError(err)) {
                pErr->code = err.code;
                return;
            }

            Lentil_Reso_addFace(model, face);
            hasFace = true;
        } else {
            if (Lentil_Core_debugLevel(-1) > 0)
                printf("Unrecognized token in model: %s\n", token);
        }
    }

    if (!hasFace) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Model has no defined faces.\n");

        pErr->code = Lentil_Core_MODELLOADFAILED;
        return;
    }
}

void Lentil_Reso_loadModelLent(FILE* modelFile, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {

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

    FILE* in = fopen(path, "r");
    if (in == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("modelload: error in opening file \"%s\"\n", path);

        pErr->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    if (strcmp(format, "obj") == 0)
        Lentil_Reso_loadModelObj(in, model, pErr);
    else if (strcmp(format, "lent") == 0)
        Lentil_Reso_loadModelLent(in, model, pErr);
    else {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("modelload: path \"%s\" shouldn't pass \"%s\".\n", path, format);

        pErr->code = Lentil_Core_INVALIDMODELFORMAT;
        fclose(in);
        return;
    }

    fclose(in);
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
