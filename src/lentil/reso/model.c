#include "model.h"

//////////////
// Includes //
#include "../core/array.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//////////
// Code //

// Creating and returning a new model.
Lentil_Reso_Model* Lentil_Reso_Model_new() {
    Lentil_Reso_Model* model = malloc(sizeof(Lentil_Reso_Model));

    // Initializing all of the vertices.
    Lentil_Core_initArray(model->pVertices, Lentil_Reso_Model_PVertex);
    Lentil_Core_initArray(model->tVertices, Lentil_Reso_Model_TVertex);
    Lentil_Core_initArray(model->nVertices, Lentil_Reso_Model_NVertex);
    Lentil_Core_initArray(model->groups, Lentil_Reso_Model_Group);

    // Initializing the material.
    model->material     = NULL;
    model->materialPath = NULL;

    return model;
}

// Destroying a model and its associated resources.
void Lentil_Reso_Model_destroy(Lentil_Reso_Model* model) {
    // Freeing all of the vertices.
    free(model->pVertices);
    free(model->tVertices);
    free(model->nVertices);

    // Freeing the memory associated with the groups.
    for (int i = 0; i < model->groupsLength; i++) {
        for (int j = 0; j < model->groups[i].facesLength; j++)
            free(model->groups[i].faces[j].triads);

        free(model->groups[i].faces);
    }
    free(model->groups);

    // Freeing the material if this model owns it.
    if (model->ownsMaterial && model->material != NULL)
        free(model->material);

    // Freeing the pointer to the model itself.
    free(model);
}

// Printing out all of the data inside of a .obj file.
void Lentil_Reso_Model_print(Lentil_Reso_Model* model) {
    printf("Model print:\n");

    // p vertices
    for (int i = 0; i < model->pVerticesLength; i++)
        printf(" vp %f %f %f %f\n", model->pVertices[i].x, model->pVertices[i].y, model->pVertices[i].z, model->pVertices[i].w);

    // t vertices
    for (int i = 0; i < model->tVerticesLength; i++)
        printf(" vt %f %f %f\n", model->tVertices[i].x, model->tVertices[i].y, model->tVertices[i].w);

    // n vertices
    for (int i = 0; i < model->nVerticesLength; i++)
        printf(" vn %f %f %f\n", model->nVertices[i].x, model->nVertices[i].y, model->nVertices[i].z);

    // Printing out the material path.
    printf(" Material path: %s\n", model->materialPath);

    // Printing out the groups.
    for (int i = 0; i < model->groupsLength; i++) {
        printf(" g\n  Name : %s\n  Mater: %s\n  Faces:\n", model->groups[i].name, model->groups[i].material);
        for (int j = 0; j < model->groups[i].facesLength; j++) {
            printf("   f");
            for (int k = 0; k < model->groups[i].faces[j].triadsLength; k++)
                printf("%d/%d/%d ", model->groups[i].faces[j].triads[k].pos, model->groups[i].faces[j].triads[k].tex, model->groups[i].faces[j].triads[k].nor);
            printf("\n");
        }
    }
}
