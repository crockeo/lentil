#include "objloader.h"

//////////////
// Includes //
#include <stdlib.h>
#include <string.h>

#include "../core/array.h"
#include "../core/debug.h"
#include "file.h"

//////////
// Code //

// Parsing out a single triad from a given FILE.
void Lentil_Reso_ObjLoader_loadTriad(FILE* file, Lentil_Reso_Model_Triad* triad, Lentil_Core_Error* pErr) {
    // TODO: Load a triad.
}

// Parsing out a single face from a given FILE.
void Lentil_Reso_ObjLoader_loadFace(FILE* file, Lentil_Reso_Model_Face* face, Lentil_Core_Error *pErr) {
    // TODO: Load a face.
}

// Attempting to load a model from Wavefront .obj file. Lentil only supports
// a subset of .obj files:
//
// * Vertices         (v)
// * Texture vertices (vt)
// * Normal vertices  (vn)
// * Material loading (mtllib)
// * Material use     (usemtl)
// * Groups           (g)
// * Faces            (f)
void Lentil_Reso_loadObjModel(FILE* file, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    if (file == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Invalid model file.\n");

        pErr->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    int currentGroup = -1,
        tokenSize    = 64;

    char* token = malloc(tokenSize * sizeof(char));
    while (Lentil_Reso_loadToken(file, token, tokenSize)) {
        if (strcmp(token, "v") == 0) {
            float x, y, z, w;
            int n = fscanf(file, "%f %f %f %f", &x, &y, &z, &w);
            if (n < 3 || n > 4) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("Invalid vertex definition.");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                free(token);
                return;
            }

            if (n == 3)
                w = 1.0f;

            Lentil_Reso_Model_PVertex vertex;

            vertex.x = x;
            vertex.y = y;
            vertex.z = z;
            vertex.w = w;

            Lentil_Core_addElement(model->pVertices, vertex, Lentil_Reso_Model_PVertex);
        } else if (strcmp(token, "vt") == 0) {
            float x, y, w;
            int n = fscanf(file, "%f %f %f", &x, &y, &w);
            if (n < 2 || n > 3) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("Invalid texture vertex definition.");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                free(token);
                return;
            }

            if (n == 2)
                w = 1.0f;

            Lentil_Reso_Model_TVertex vertex;

            vertex.x = x;
            vertex.y = y;
            vertex.w = w;

            Lentil_Core_addElement(model->tVertices, vertex, Lentil_Reso_Model_TVertex);
        } else if (strcmp(token, "vn") == 0) {
            float x, y, z;
            int n = fscanf(file, "%f %f %f", &x, &y, &z);
            if (n != 3) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("Invalid normal texture definition.");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                free(token);
                return;
            }

            Lentil_Reso_Model_NVertex vertex;

            vertex.x = x;
            vertex.y = y;
            vertex.z = z;

            Lentil_Core_addElement(model->nVertices, vertex, Lentil_Reso_Model_NVertex);
        } else if (strcmp(token, "mtllib") == 0) {
            // TODO: Load a .mtl file.
            /*Lentil_Reso_loadToken(file, token, tokenSize);*/
            /*model->material = Lentil_Reso_loadMaterial(*/
            /*Lentil_Reso_consumeLine(file);*/
        } else if (strcmp(token, "usemtl") == 0) {
            if (currentGroup == -1) {
                if (Lentil_Core_debugLevel(-1) > 0)
                    printf("You must define a group before using usemtl.");

                pErr->code = Lentil_Core_MODELLOADFAILED;
                free(token);
                return;
            }

            Lentil_Reso_loadToken(file, token, tokenSize);

            int slen = strlen(token);
            model->groups[currentGroup].material = malloc((slen + 1) * sizeof(char));
            strncpy(model->groups[currentGroup].material, token, slen);
            model->groups[currentGroup].material[slen + 1] = '\0';
        } else if (strcmp(token, "g") == 0) {
            // Getting the group name.
            Lentil_Reso_loadToken(file, token, tokenSize);

            // Constructing the group.
            int slen = strlen(token);
            Lentil_Reso_Model_Group group;

            group.name = malloc((slen + 1) * sizeof(char));
            strncpy(group.name, token, slen);
            group.name[slen] = '\0';

            group.material = NULL;

            // Adding the group.
            Lentil_Core_addElement(model->groups, group, Lentil_Reso_Model_Group);
            currentGroup++;
        } else if (strcmp(token, "f") == 0) {
            Lentil_Reso_Model_Face face;
            Lentil_Reso_ObjLoader_loadFace(file, &face, pErr);
            if (Lentil_Core_isError(*pErr)) {
                free(token);
                return;
            }

            Lentil_Core_addElement(model->groups[currentGroup].faces, face, Lentil_Reso_Model_Face);
        } else
            Lentil_Reso_consumeLine(file);
    }
}

// Effectivel the same thing as calling Lentil_Reso_loadObjModel(fopen(path), "r").
// Refer to the documentation of the above for use.
void Lentil_Reso_loadObjModelStr(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    Lentil_Reso_loadObjModel(fopen(path, "r"), model, pErr);
}
