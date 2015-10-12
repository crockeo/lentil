#include "objloader.h"

//////////////
// Includes //
#include <stdlib.h>
#include <string.h>

#include "../core/debug.h"
#include "file.h"

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
void Lentil_Reso_loadObjModel(FILE* file, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    if (file == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Invalid model file.\n");

        pErr->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    int tokenSize = 64;
    char* token = malloc(tokenSize * sizeof(char));
    while (Lentil_Reso_loadToken(file, token, tokenSize)) {
        if (strcmp(token, "v") == 0) {
            float x, y, z, w;
            int n = fscanf(file, "%f %f %f %f", &x, &y, &z, &w);
            if (n < 3 || n > 4) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("Invalid vertex definition.");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                return;
            }

            if (n == 3)
                w = 1.0f;

            // TODO: Add vertex.
        } else if (strcmp(token, "vt") == 0) {
            float x, y, w;
            int n = fscanf(file, "%f %f %f", &x, &y, &w);
            if (n < 2 || n > 3) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("Invalid texture vertex definition.");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                return;
            }

            if (n == 2)
                w = 1.0f;

            // TODO: Add texture vertex.
        } else if (strcmp(token, "vn") == 0) {
            float x, y, z;
            int n = fscanf(file, "%f %f %f", &x, &y, &z);
            if (n != 3) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("Invalid normal texture definition.");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                return;
            }

            // TODO: Add normal vertex.
        } else if (strcmp(token, "mtllib") == 0) {
            /*Lentil_Reso_loadToken(file, token, tokenSize);*/
            /*model->material = Lentil_Reso_loadMaterial(*/
            /*Lentil_Reso_consumeLine(file);*/
        } else if (strcmp(token, "usemtl") == 0) {
            Lentil_Reso_consumeLine(file);
        } else if (strcmp(token, "g") == 0) {
            Lentil_Reso_consumeLine(file);
        } else if (strcmp(token, "f") == 0) {
            Lentil_Reso_consumeLine(file);
        } else
            Lentil_Reso_consumeLine(file);
    }
}

// Effectivel the same thing as calling Lentil_Reso_loadObjModel(fopen(path), "r").
// Refer to the documentation of the above for use.
void Lentil_Reso_loadObjModelStr(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    Lentil_Reso_loadObjModel(fopen(path, "r"), model, pErr);
}
