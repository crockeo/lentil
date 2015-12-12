#include "lntsaver.h"

//////////////
// Includes //
#include "../core/debug.h"
#include "file.h"

//////////
// Code //

// Saving a Lentil_Reso_Model* to a given file on disk in the .lnt format, a
// file format made for lentil.
//
// For file spec information, check out the documentation in doc/lntmodel.txt
void Lentil_Reso_saveLntModel(FILE* file, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    if (file == NULL) {
        if (Lentil_Core_debugLevel(-1) > 0)
            printf("Invalid file pointer.\n");

        pErr->code = Lentil_Core_FILENOTFOUND;
        return;
    }

    // Writing the groups length.
    Lentil_Reso_saveInt(file, model->groupsLength);

    // Writing the faces lengths.
    for (int i = 0; i < model->groupsLength; i++)
        Lentil_Reso_saveInt(file, model->groups[i].facesLength);

    // Writing the triads lengths.
    for (int i = 0; i < model->groupsLength; i++)
        for (int j = 0; j < model->groups[i].facesLength; j++)
            Lentil_Reso_saveInt(file, model->groups[i].faces[j].triadsLength);

    // Writing out the vertices lengths.
    Lentil_Reso_saveInt(file, model->pVerticesLength);
    Lentil_Reso_saveInt(file, model->tVerticesLength);
    Lentil_Reso_saveInt(file, model->nVerticesLength);

    // Position vertices.
    for (int i = 0; i < model->pVerticesLength; i++) {
        Lentil_Reso_saveFloat(file, model->pVertices[i].x);
        Lentil_Reso_saveFloat(file, model->pVertices[i].y);
        Lentil_Reso_saveFloat(file, model->pVertices[i].z);
        Lentil_Reso_saveFloat(file, model->pVertices[i].w);
    }

    // Texture vertices.
    for (int i = 0; i < model->tVerticesLength; i++) {
        Lentil_Reso_saveFloat(file, model->tVertices[i].x);
        Lentil_Reso_saveFloat(file, model->tVertices[i].y);
        Lentil_Reso_saveFloat(file, model->tVertices[i].w);
    }

    // Normal vertices.
    for (int i = 0; i < model->nVerticesLength; i++) {
        Lentil_Reso_saveFloat(file, model->nVertices[i].x);
        Lentil_Reso_saveFloat(file, model->nVertices[i].y);
        Lentil_Reso_saveFloat(file, model->nVertices[i].z);
    }

    // Triads.
    for (int i = 0; i < model->groupsLength; i++) {
        for (int j = 0; j < model->groups[i].facesLength; j++) {
            for (int k = 0; k < model->groups[i].faces[j].triadsLength; k++) {
                Lentil_Reso_saveInt(file, model->groups[i].faces[j].triads[k].pos);
                Lentil_Reso_saveInt(file, model->groups[i].faces[j].triads[k].tex);
                Lentil_Reso_saveInt(file, model->groups[i].faces[j].triads[k].nor);
            }
        }
    }
}

// Saving a Lentil_Reso_Model* to a given path on disk - opens a file and then
// calles Lentil_Reso_saveLntModel.
void Lentil_Reso_saveLntModelStr(const char* path, Lentil_Reso_Model* model, Lentil_Core_Error* pErr) {
    FILE* file = fopen(path, "w");
    Lentil_Reso_saveLntModel(file, model, pErr);

    if (file != NULL)
        fclose(file);
}
