#include "lntloader.h"

//////////////
// Includes //
#include <stdlib.h>

#include "../core/debug.h"
#include "file.h"

//////////
// Code //

// Loading a Lentil_Reso_Model* from a given file on disk in the .lnt format, a
// file format made for lentil.
//
// For file spec information, check out the documentation in doc/lntmodel.txt
void Lentil_Reso_loadLntModel(FILE* file, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    if (file == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Invalid file pointer.\n");

        pErr->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    // Allocating space for the groups, faces, and triads.
    Lentil_Reso_loadInt(file, &model->groupsSize);
    model->groupsLength = model->groupsSize;
    model->groups       = malloc(model->groupsSize * sizeof(Lentil_Reso_Model_Group));

    for (int i = 0; i < model->groupsLength; i++) {
        Lentil_Reso_loadInt(file, &model->groups[i].facesSize);
        model->groups[i].facesLength = model->groups[i].facesLength;
        model->groups[i].faces       = malloc(model->groups[i].facesSize * sizeof(Lentil_Reso_Model_Face));
    }

    for (int i = 0; i < model->groupsLength; i++) {
        for (int j = 0; j < model->groups[i].facesLength; j++) {
            Lentil_Reso_loadInt(file, &model->groups[i].faces[j].triadsSize);
            model->groups[i].faces[j].triadsLength = model->groups[i].faces[j].triadsSize;
            model->groups[i].faces[j].triads       = malloc(model->groups[i].faces[j].triadsSize * sizeof(Lentil_Reso_Model_Triad));
        }
    }

    // Allocating space for the vertices.
    Lentil_Reso_loadInt(file, &model->pVerticesSize);
    model->pVerticesLength = model->pVerticesSize;
    model->pVertices       = malloc(model->pVerticesSize * sizeof(Lentil_Reso_Model_PVertex));

    Lentil_Reso_loadInt(file, &model->tVerticesSize);
    model->tVerticesLength = model->tVerticesSize;
    model->tVertices       = malloc(model->tVerticesSize * sizeof(Lentil_Reso_Model_TVertex));

    Lentil_Reso_loadInt(file, &model->nVerticesSize);
    model->nVerticesLength = model->nVerticesSize;
    model->nVertices       = malloc(model->nVerticesSize * sizeof(Lentil_Reso_Model_NVertex));

    float x, y, z, w;
    // Loading the position vertices.
    Lentil_Reso_Model_PVertex p;
    for (int i = 0; i < model->pVerticesLength; i++) {
        Lentil_Reso_loadFloat(file, &x);
        Lentil_Reso_loadFloat(file, &y);
        Lentil_Reso_loadFloat(file, &z);
        Lentil_Reso_loadFloat(file, &w);

        p.x = x;
        p.y = y;
        p.z = z;
        p.w = w;

        model->pVertices[i] = p;
    }

    // Loading the texture vertices.
    Lentil_Reso_Model_TVertex t;
    for (int i = 0; i < model->tVerticesLength; i++) {
        Lentil_Reso_loadFloat(file, &x);
        Lentil_Reso_loadFloat(file, &y);
        Lentil_Reso_loadFloat(file, &w);

        t.x = x;
        t.y = y;
        t.w = w;

        model->tVertices[i] = t;
    }

    // Loading the normal vertices.
    Lentil_Reso_Model_NVertex n;
    for (int i = 0; i < model->nVerticesLength; i++) {
        Lentil_Reso_loadFloat(file, &x);
        Lentil_Reso_loadFloat(file, &y);
        Lentil_Reso_loadFloat(file, &z);

        n.x = x;
        n.y = y;
        n.z = z;

        model->nVertices[i] = n;
    }

    // Loading all of the triads.
    Lentil_Reso_Model_Triad triad;
    int pos, tex, nor;
    for (int i = 0; i < model->groupsLength; i++) {
        for (int j = 0; j < model->groups[i].facesLength; j++) {
            for (int k = 0; k < model->groups[i].faces[j].triadsLength; k++) {
                Lentil_Reso_loadInt(file, &pos);
                Lentil_Reso_loadInt(file, &tex);
                Lentil_Reso_loadInt(file, &nor);

                triad.pos = pos;
                triad.tex = tex;
                triad.nor = nor;

                model->groups[i].faces[j].triads[k] = triad;
            }
        }
    }
}

// Loading a Lentil_Reso_Model* from a given path on disk - opens a file and
// then calls Lentil_Reso_loadLntModel.
void Lentil_Reso_loadLntModelStr(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    FILE* file = fopen(path, "r");
    Lentil_Reso_loadLntModel(file, model, pErr);

    if (file != NULL)
        fclose(file);
}
